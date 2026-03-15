#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"

ScriptEventList* TESObjectREFR::GetEventList() const
{
	BSExtraData* xData = extraDataList.GetByType(kExtraData_Script);
	if (xData)
	{
		ExtraScript* xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript);
		if (xScript)
			return xScript->eventList;
	}

	return 0;
}

NiAVObject* TESObjectREFR::GetNiBlock2(const char* blockName) const
{
	if (this->loadedData && this->loadedData->rootNode && *blockName) {
		return this->loadedData->rootNode->GetBlock(blockName);
	}
	return nullptr;
}

hkpRigidBody* TESObjectREFR::GetRigidBody(const char* blockName) const
{
	NiAVObject* block = this->GetNiBlock2(blockName);
	if (block && block->m_collisionObject && block->m_collisionObject->worldObj && block->m_collisionObject->worldObj->phkObject) {
		hkpRigidBody* rigidBody = (hkpRigidBody*)block->m_collisionObject->worldObj->phkObject;
		if (rigidBody->m_collidable.m_broadPhaseHandle.m_type == 1) {
			return rigidBody;
		}
	}
	return nullptr;
}

bhkCharacterController* TESObjectREFR::GetCharacterController() const
{
	BaseProcess* proc = ((Actor*)this)->baseProcess;
	if (proc->uiProcessLevel > 1) return nullptr;
	bhkCharacterController* charCtrl = proc->GetCharacterController();
	if (!charCtrl) return nullptr;
	return charCtrl;
}

void TESObjectREFR::MoveToCell(TESObjectCELL* cell, const NiVector3& posVector)
{
	TESObjectCELL* parentCell;
	TESObjectREFR::RenderState* renderState; 
	TESWorldSpace* worldSpace; 
	parentCell = this->parentCell;
	renderState = this->loadedData;
	worldSpace = cell->worldSpace;
	if (parentCell != nullptr && renderState != nullptr && renderState->rootNode != nullptr
		&& (parentCell == cell || worldSpace != nullptr && parentCell->worldSpace == worldSpace))
	{
		TESObjectREFR::SetPos(posVector);
	}
	else
	{
		s_tempPosMarker->parentCell = cell;
		s_tempPosMarker->rotation.x = this->rotation.x;
		s_tempPosMarker->rotation.y = this->rotation.y;
		s_tempPosMarker->rotation.z = this->rotation.z;
		s_tempPosMarker->position.x = posVector.x;
		s_tempPosMarker->position.y = posVector.y;
		s_tempPosMarker->position.z = posVector.z;
		CdeclCall<void>(0x528730, this, s_tempPosMarker, 0, 0, 0);
	}
}

void TESObjectREFR::SetPos(const NiVector3& posVector)
{
	// TESObjectREFR::SetLocation(TESObjectREFR *this, NiPoint3 *a2)
	ThisCall(0x4EEAE0, this, &posVector);

	RenderState* rs = loadedData;
	if (!rs)
		return;
	NiNode* rootNode = rs->rootNode;
	if (!rootNode)
		return;

	rootNode->m_localTranslate.x = position.x;
	rootNode->m_localTranslate.y = position.y;
	rootNode->m_localTranslate.z = position.z;

	// 4. Handle collision update based on Actor vs non-Actor
	if (IsActor())
	{
		Actor* actor = (Actor*)this;
		BaseProcess* process = actor->baseProcess;

		if (process && process->uiProcessLevel <= 1)
		{
			bhkCharacterController* charCtrl =
				((MiddleHighProcess*)process)->spCharCtrl;

			if (charCtrl && charCtrl->chrContext.hkState != 4)
			{
				bhkCachingShapePhantom* bhkPhantom = charCtrl->spShapePhantom;

				if (bhkPhantom)
				{
					hkpCachingShapePhantom* hkPhantom =
						(hkpCachingShapePhantom*)bhkPhantom->phkObject;

					if (hkPhantom)
					{
						hkMotionState* ms = &hkPhantom->m_motionState;

						// Convert Gamebryo ? Havok units (scale ? 1/70)
						__m128 hkPos = _mm_mul_ps(
							_mm_loadu_ps((float*)&position),
							_mm_set1_ps(0.01428571f)
						);

						// Apply rotation center offset: hkPos += rot * rotCenter
						hkVector4& rotCenter = charCtrl->kRotCenter;
						hkMatrix3x4& rot = ms->transform.rotation;
						__m128 rc = _mm_load_ps((float*)&rotCenter);

						hkPos = _mm_add_ps(hkPos, _mm_mul_ps(
							_mm_shuffle_ps(rc, rc, 0x00),
							_mm_load_ps(&rot.cr[0][0])));
						hkPos = _mm_add_ps(hkPos, _mm_mul_ps(
							_mm_shuffle_ps(rc, rc, 0x55),
							_mm_load_ps(&rot.cr[1][0])));
						hkPos = _mm_add_ps(hkPos, _mm_mul_ps(
							_mm_shuffle_ps(rc, rc, 0xAA),
							_mm_load_ps(&rot.cr[2][0])));


						hkpCharacterProxy* hkProxy =
							(hkpCharacterProxy*)charCtrl->phkObject;

						float extraTolerance = hkProxy->m_keepDistance + hkProxy->m_keepContactTolerance;


						_mm_store_ps((float*)&hkProxy->m_oldDisplacement, _mm_setzero_ps());


						hkpShapePhantom* proxyPhantom = hkProxy->shapePhantom;

						hkVector4* phantomTranslation =
							&proxyPhantom->m_motionState.transform.translation;

						_mm_store_ps((float*)phantomTranslation, hkPos);

						// hkpShapePhantom::setPosition(this=proxyPhantom, &translation, extraTolerance)
						ThisCall(0x90DDA0, proxyPhantom, phantomTranslation, extraTolerance);
					}
				}
			}
		}
	}
	else
	{
		// Non-actor: reset collision to match new transform
		rootNode->ResetCollision();
	}

	// 5. Propagate transform change through the scene graph
	NiUpdateData updateData;
	rootNode->UpdateDownwardPass(updateData, 0);
}


bool TESObjectREFR::CanHaveSound() const
{
	if (!baseForm)
	{
		return false;
	}
	switch (baseForm->typeID)
	{
		case kFormType_Sound:
		case kFormType_Activator:
		case kFormType_Terminal:
		case kFormType_Door:
		case kFormType_Light:
		case kFormType_MoveableStatic:
		case kFormType_PlaceableWater:
			return true;
		default:
			return false;
	}
}

void TESObjectREFR::AttachSound(bool bAttach)
{
	ThisCall(0x4F5F00, this, bAttach);
}

static PlayerCharacter ** g_thePlayer = (PlayerCharacter **)0x0107A104;


PlayerCharacter* PlayerCharacter::GetSingleton()
{
	return *g_thePlayer;
}


float GetDistance(TESObjectREFR* ref1, TESObjectREFR* ref2) {
	return (float)ThisCall<double>(0x4EDBF0, ref1, ref2, 0, 0);
}
float GetThresholdedAV(Actor* actor, int avCode) {
	return ThisCall<float>(0x5A28E0, &actor->avOwner, avCode);
}

float Actor::GetRadiationLevel(bool scaleByResist)
{
	float result = 0;
	if (inWater)
	{
		TESWaterForm* waterForm = nullptr;
		if (loadedData && loadedData->currWaterRef)
		{
			waterForm = ((BGSPlaceableWater*)loadedData->currWaterRef->baseForm)->water;
			if (waterForm && waterForm->waterForm)
				waterForm = waterForm->waterForm;
		}
		else {
			waterForm = ThisCall<TESWaterForm*>(0x4D5710, parentCell);
		}
		if (waterForm && waterForm->radiation) {
			result = waterForm->radiation * (isSwimming ? *(float*)(0xF620CC+4) : *(float*)(0xF620D8+4));
		}
	}
	for (auto iter = LoadedReferenceCollection::Get()->radiationEmitters.Begin(); !iter.Done(); iter.Next()) {
		if (TESObjectREFR* refr = iter.Get()) {
			if (auto xRadius = (ExtraRadius*)(refr->extraDataList.GetByType(kExtraData_Radius))) {
				float distance = xRadius->radius - GetDistance((TESObjectREFR*)this, refr);
				if (distance > 0) {
					if (auto xRadiation = (ExtraRadiation*)(refr->extraDataList.GetByType(kExtraData_Radiation))) {
						result += xRadiation->radiation * distance / xRadius->radius;
					}
				}
			}
		}
	}
	if (result && scaleByResist)
		result *= 1.0F - (GetThresholdedAV(this, eActorVal_RadResistance) * 0.01F);
	if (result < 0.0) result = 0.0;
	return result;
}
TESActorBase* Actor::GetActorBase()

{
	ExtraLeveledCreature* xLvlCre = (ExtraLeveledCreature*)this->extraDataList.GetByType(kExtraData_LeveledCreature);
	return (xLvlCre && xLvlCre->form) ? (TESActorBase*)xLvlCre->form : (TESActorBase*)baseForm;
}

TESIdleForm* AnimData::GetPlayedIdle() const
{
	AnimIdle* queuedIdleAnim = this->queuedIdleAnim;
	if (queuedIdleAnim && queuedIdleAnim->idleForm)
		return queuedIdleAnim->idleForm;
	if (this->idleAnim && this->idleAnim->idleForm) {
		bool noIdlePlaying = ThisCall<bool>(0x45E180, this);
		if (!noIdlePlaying) return this->idleAnim->idleForm;
	}
	return nullptr;
}
