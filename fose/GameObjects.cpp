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
		s_tempPosMarker->rotX = this->rotX;
		s_tempPosMarker->rotY = this->rotY;
		s_tempPosMarker->rotZ = this->rotZ;
		s_tempPosMarker->posX = posVector.x;
		s_tempPosMarker->posY = posVector.y;
		s_tempPosMarker->posZ = posVector.z;
		CdeclCall<void>(0x528730, this, s_tempPosMarker, 0, 0, 0);
	}
}

void TESObjectREFR::SetPos(const NiVector3& posVector)
{
		
		ThisCall<void>(0x4EEAE0, this, &posVector);
		MobileObject* mobileObj = DYNAMIC_CAST(this, TESObjectREFR, MobileObject);
		if (mobileObj)
		{
			bhkCharacterController* ctrl = mobileObj->GetCharacterController();
			if (ctrl && ctrl->chrContext.hkState != 4)
			{
					ThisCall<void>(0x4E6580, ctrl, &posVector);
			}
		}
		NiNode* node = this->GetNiNode();
		if (node)
		{
			node->m_localTranslate = posVector;
			CdeclCall<void>(0x8DA3B0, node, 1);
			NiUpdateData updateData;
			ThisCall<void>(0x8264C0, node, &updateData);
		}
		
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
