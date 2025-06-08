#include "functions.h"
#include <string>
#include <unordered_map>
#include "CommandTable.h"
#include "GameAPI.h"
#include "GameEffects.h"
#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"
#include "SafeWrite.h"
#include "GameMenus.h"

extern int g_version;

extern char* s_strArgBuffer;
extern char* s_strValBuffer;

extern std::unordered_map<UInt32, char*> markerIconMap;

extern CommandInfo* cmd_IsKeyPressed;

extern TESClimate* s_forcedClimate;

static bool timePatched = false;

const UInt32 kMsgIconsPathAddr[] = { 0xDC0C38, 0xDC0C78, 0xDC5544, 0xDCE658, 0xDD9148, 0xDE3790, 0xDF3278 };

TESObjectREFR* s_tempPosMarker;

bool Cmd_GetFormFlag_Execute(COMMAND_ARGS)
{
	TESForm* form = nullptr;
	UInt32 flag;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &form, &flag) && (form->flags & flag))
	{
		*result = 1;
	}
	if (IsConsoleMode()) Console_Print("GetFormFlag >> %.f", *result);
	return true;
}

bool Cmd_SetFormFlag_Execute(COMMAND_ARGS)
{
	TESForm* form;
	UInt32 flag, inval;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &form, &flag, &inval))
	{
		if (inval) {
			form->flags |= flag;
		}
		else {
			form->flags &= ~flag;
		}
		*result = 1;
	}
	return true;
}

bool Cmd_SetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	TESLandTexture* landTex = nullptr;
	BGSTextureSet* texSet = nullptr;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &landTex, &texSet)) {
		landTex->textureSet = texSet;
		*result = 1;
	}
	return true;
}

bool Cmd_ForceClimate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate* climate = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &climate) && (!climate || IS_TYPE(climate, TESClimate))) {
		s_forcedClimate = climate;
		*result = 1;
	}
	return true;
}

bool Cmd_IsSpellTargetAlt_Execute(COMMAND_ARGS)
{
	*result = 0;
	MagicItem* magicItem = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &magicItem) && thisObj->IsActor()) {
		Actor* actor = (Actor*)thisObj;
		for (auto iter = actor->magicTarget.GetEffectList()->Head(); iter; iter = iter->next) {
			if (ActiveEffect* activeEff = iter->data; activeEff && (activeEff->magicItem == magicItem) && activeEff->bActive && !activeEff->bTerminated) {
				*result = 1;
				break;
			}
		}
	}
	if (IsConsoleMode()) Console_Print("IsSpellTargetAlt >> %.f", *result);
	return true;
}
bool Cmd_GetActiveMenuMode_Execute(COMMAND_ARGS)
{
	*result = CdeclCall<int>(0x6191A0);
	return true;
}

bool Cmd_GetCellImageSpace_Execute(COMMAND_ARGS)
{
	TESObjectCELL* cell = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &cell) && IS_TYPE(cell, TESObjectCELL)) {
		ExtraCellImageSpace* xCellIS = (ExtraCellImageSpace*)cell->extraDataList.GetByType(kExtraData_CellImageSpace);
		if (xCellIS && xCellIS->imageSpace) {
			*(UInt32*)result = xCellIS->imageSpace->refID;
			if (IsConsoleMode()) Console_Print("GetCellImageSpace >> 0x%X", *result);
		}
	}
	return true;
}

bool Cmd_GetCurrentClimate_Execute(COMMAND_ARGS)
{
	Sky* currentSky = Sky::Get();
	TESClimate* climate = currentSky->currClimate;
	if (climate)
		*(UInt32*)result = climate->refID;
	if (IsConsoleMode()) {
		Console_Print("GetCurrentClimate >> 0x%X", climate->refID);
	}
	return true;
}

bool Cmd_SetCurrentClimate_Execute(COMMAND_ARGS)
{
	TESClimate* climate;
	if (ExtractArgs(EXTRACT_ARGS, &climate) && IS_TYPE(climate, TESClimate))
	{
		Sky* currentSky = Sky::Get();
		currentSky->currClimate = climate;
	}
	return true;
}

bool Cmd_RefreshCurrentClimate_Execute(COMMAND_ARGS)
{
	*result = 0;
	Sky* currentSky = Sky::Get();
	TESClimate* climate = s_forcedClimate ? s_forcedClimate : currentSky->currClimate;
	currentSky->RefreshClimate(climate, true);
	*result = 1;
	return true;
}

bool Cmd_GetClimateTraitNumeric_Execute(COMMAND_ARGS)
{
	TESClimate* climate;
	UInt32 traitID;
	if (ExtractArgs(EXTRACT_ARGS, &climate, &traitID) && IS_TYPE(climate, TESClimate)) {
		switch (traitID)
		{
		case 0:
			*result = climate->sunriseBegin * 10;
			break;
		case 1:
			*result = climate->sunriseEnd * 10;
			break;
		case 2:
			*result = climate->sunsetBegin * 10;
			break;
		case 3:
			*result = climate->sunsetEnd * 10;
			break;
		case 4:
			*result = climate->volatility;
			break;
		case 5:
			*result = climate->phaseLength >> 6;
			break;
		case 6:
			*result = climate->phaseLength & 63;
			break;
		default:
			break;
		}
		if (IsConsoleMode()) Console_Print("GetClimateTraitNumeric %d >> %.2f", traitID, *result);
	}
	return true;
}

bool Cmd_SetClimateTraitNumeric_Execute(COMMAND_ARGS)
{
	TESClimate* climate;
	UInt32 traitID, val;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &climate, &traitID, &val) && IS_TYPE(climate, TESClimate)) {
		switch (traitID)
		{
		case 0:
			climate->sunriseBegin = (val > 1430) ? 143 : (val / 10);
			break;
		case 1:
			climate->sunriseEnd = (val > 1430) ? 143 : (val / 10);
			break;
		case 2:
			climate->sunsetBegin = (val > 1430) ? 143 : (val / 10);
			break;
		case 3:
			climate->sunsetEnd = (val > 1430) ? 143 : (val / 10);
			break;
		case 4:
			climate->volatility = (val > 255) ? 255 : val;
			break;
		case 5:
			climate->phaseLength = (climate->phaseLength & 63) + ((val > 3) ? 192 : (val << 6));
			break;
		case 6:
			climate->phaseLength = (climate->phaseLength & 192) + ((val > 63) ? 63 : val);
			break;
		default:
			break;
		}
	}
	return true;
}

bool Cmd_SetClimateSunTexture_Execute(COMMAND_ARGS)
{
	TESClimate* climate;
	char path[MAX_PATH];
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &climate, &path) && IS_TYPE(climate, TESClimate)) {
		climate->sunTexture.ddsPath.Set(path);
		*result = 1;
	}
	return true;
}

bool Cmd_SetClimateSunGlareTexture_Execute(COMMAND_ARGS)
{
	TESClimate* climate;
	char path[MAX_PATH];
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &climate, &path) && IS_TYPE(climate, TESClimate)) {
		climate->sunGlareTexture.ddsPath.Set(path);
		*result = 1;
	}
	return true;
}

bool Cmd_GetGameVolume_Execute(COMMAND_ARGS)
{
	UInt32 volType;
	if (ExtractArgs(EXTRACT_ARGS, &volType) && (volType <= 5)) {
		*result = BSAudioManager::Get()->volumes[volType] * 100;
		if (IsConsoleMode()) Console_Print("GetGameVolume %d >> %.2f", volType, *result);
	}
	return true;
}

bool Cmd_SetGameVolume_Execute(COMMAND_ARGS)
{
	UInt32 volType;
	int volLevel = -1;
	if (ExtractArgs(EXTRACT_ARGS, &volType, &volLevel) && (volType <= 5) && (volLevel <= 100)) {
		BSAudioManager::Get()->volumes[volType] = (volLevel < 0) ? *(float*)(0x117910C + volType * 0xC) : (volLevel * 0.01F);
	}
	return true;
}

bool Cmd_MoveToCell_Execute(COMMAND_ARGS)
{
	TESObjectCELL* cell;
	NiVector3 posVector;
	if (s_tempPosMarker == nullptr) {
		TESObjectREFR* ref = (TESObjectREFR*)GameHeapAlloc(sizeof(TESObjectREFR));
		s_tempPosMarker = ThisCall<TESObjectREFR*>(0x4F9970, ref);
		ThisCall(0x4553B0, s_tempPosMarker);
	}
	if (ExtractArgs(EXTRACT_ARGS, &cell, &posVector.x, &posVector.y, &posVector.z))
	{
		if NOT_ID(cell, Cell)
		{
			if NOT_ID(cell, WorldSpace)
				return true;
			cell = ((TESWorldSpace*)cell)->cell;
		}
		thisObj->MoveToCell(cell, posVector);
		*result = 1;
	}
	return true;
}

void SwapSlash(char* str) {
	char* current_pos = strchr(str, '/');
	for (char* p = current_pos; (current_pos = strchr(str, '/')) != nullptr; *current_pos = '\\');
}
bool Cmd_AddTileFromTemplate_Execute(COMMAND_ARGS)
{
	if (!ExtractFormatStringArgs(0, s_strValBuffer, EXTRACT_ARGS_EX, kCommandInfo_MessageBoxEx.numParams))
		return true;

	char* tempName = GetNextToken(s_strValBuffer, '|');
	if (!*tempName) return true;
	char* altName = GetNextToken(tempName, '|');
	TileMenu* menu;
	Tile* component = nullptr;
	SwapSlash(s_strValBuffer);
	char* slashPos = (char*)strchr(s_strValBuffer, '\\');
	if (slashPos)
	{
		*slashPos = 0;
		menu = TileMenu::GetMenuTile(s_strValBuffer);
		if (!menu) return true;
		const char* trait = nullptr;
		component = menu->GetComponent(slashPos + 1, &trait);
		if (trait) return true;
	}
	else
	{
		menu = TileMenu::GetMenuTile(s_strValBuffer);
		component = menu;
	}
	if (component)
	{
		component = menu->menu->AddTileFromTemplate(component, tempName);
		if (component)
		{
			*result = 1;
			if (*altName)
				component->name.Set(altName);
		}
	}

	return true;
}

bool Hook_GetTeleportCell_Execute(COMMAND_ARGS)
{
	UInt32* refResult = (UInt32*)result;
	*refResult = 0;

	if (!thisObj)
		return true;

	ExtraTeleport* xTele = GetByTypeCast(thisObj->extraDataList, Teleport);
	// parentCell will be null if linked door's cell is not currently loaded (e.g. most exterior cells)
	if (xTele && xTele->data && xTele->data->linkedDoor && xTele->data->linkedDoor->parentCell) {
		*refResult = xTele->data->linkedDoor->parentCell->refID;
	}

	return true;
}
bool Hook_GetRepairList_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm* pForm = 0;

	if (!ExtractArgs(EXTRACT_ARGS, &pForm)) return true;
	pForm = pForm->TryGetREFRParent();
	if (!pForm) {
		if (!thisObj) return true;
		pForm = thisObj->baseForm;
	}
	BGSRepairItemList* pRepairList = DYNAMIC_CAST(pForm, TESForm, BGSRepairItemList);
	if (pRepairList && pRepairList->listForm) {
		*((UInt32*)result) = pRepairList->listForm->refID;
	}
	return true;
}

bool Cmd_SetUIFloatGradual_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	float startVal, endVal, timer;
	UInt32 changeMode = 0;
	UInt8 numArgs = scriptData[*opcodeOffsetPtr];
	if (ExtractArgs(EXTRACT_ARGS, &tilePath, &startVal, &endVal, &timer, &changeMode))
	{
		Tile::Value* tileVal = nullptr;
		if (Tile* component = Tile::GetTargetComponent(tilePath, &tileVal)) {
			if (numArgs >= 4)
			{
				if (changeMode > 4) {
					changeMode = 0;
				}
				else {
					UInt8 changeModeMatch[] = { 0, 4, 1, 5, 6 };
					changeMode = changeModeMatch[changeMode];
				}
				component->StartGradualSetFloat(tileVal->id, startVal, endVal, timer, changeMode);
			}
			else
			{
				component->EndGradualSetFloat(tileVal->id);
				if (numArgs >= 2)
					tileVal->SetFloat(startVal);
			}
		}
	}
	return true;
}

bool Cmd_IsIdlePlayingEx_Execute(COMMAND_ARGS)
{
	TESIdleForm* idleAnim;
	if (ExtractArgs(EXTRACT_ARGS, &idleAnim)) {
		AnimData* animData = thisObj->GetAnimData();
		if (animData && (animData->GetPlayedIdle() == idleAnim)) {
			*result = 1;
		}
	}
	return true;
}

bool Cmd_GetPlayedIdle_Execute(COMMAND_ARGS)
{
	if (AnimData* animData = thisObj->GetAnimData()) {
		if (TESIdleForm* idleAnim = animData->GetPlayedIdle()) {
			*(UInt32*)result = idleAnim->refID;
		}
	}
	return true;
}

bool Cmd_PlayIdleEx_Execute(COMMAND_ARGS)
{
	TESIdleForm* idleAnim = nullptr;
	Actor* actor = (Actor*)thisObj;
	if (actor->baseProcess && !actor->baseProcess->uiProcessLevel && ExtractArgs(EXTRACT_ARGS, &idleAnim))
		if (AnimData* animData = thisObj->GetAnimData())
		{
			if (!idleAnim)
				idleAnim = ThisCall<TESIdleForm*>(0x553D70, (UInt32*)0x10721B8, actor, ((HighProcess*)actor->baseProcess)->pTarget);
			// todo from here
			else if (idleAnim->children)
				idleAnim = idleAnim->FindIdle(actor);
			if (idleAnim && (animData->GetPlayedIdle() != idleAnim))
				animData->PlayIdle(idleAnim);
		}
	return true;
}

bool Cmd_GetCreatureType_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESCreature* creature = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &creature)) {
		if (creature || (thisObj && thisObj->IsActor() && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase()))) {
			if IS_ID(creature, Creature)
			{
				*result = creature->type;
				if (IsConsoleMode()) {
					Console_Print("GetCreatureType >> %.f", *result);
				}
			}
		}
	}
	return true;
}

bool Cmd_IsInWater_Execute(COMMAND_ARGS)
{
	*result = 0;
	if ((thisObj->IsActor()) && ((Actor*)thisObj)->inWater) {
		*result = 1;
	}
	return true;
}


bool Cmd_GetActorGravityMult_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (bhkCharacterController* charCtrl = thisObj->GetCharacterController()) {
		*result = charCtrl->gravity;
		if (IsConsoleMode()) {
			Console_Print("GetActorGravityMult >> %f", *result);
		}
	}
	return true;
}

bool Cmd_SetActorGravityMult_Execute(COMMAND_ARGS)
{
	float gravityMult;
	if (ExtractArgs(EXTRACT_ARGS, &gravityMult)) {
		if (bhkCharacterController* charCtrl = thisObj->GetCharacterController()) {
			charCtrl->gravity = gravityMult;
		}
	}
	return true;
}


bool Cmd_GetFallTimeElapsed_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (bhkCharacterController* charCtrl = thisObj->GetCharacterController())
		*result = charCtrl->fallTime;
	return true;
}

bool Cmd_GetFallTimeRemaining_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (bhkCharacterController* charCtrl = thisObj->GetCharacterController())
		*result = charCtrl->calculatePitchTimer;
	return true;
}

bool Cmd_SetActorVelocity_Execute(COMMAND_ARGS)
{
	char axis;
	float velocity;
	if (ExtractArgs(EXTRACT_ARGS, &axis, &velocity)) {
		if (bhkCharacterController* charCtrl = thisObj->GetCharacterController()) {
			if (charCtrl->phkObject) {
				((hkpCharacterProxy*)charCtrl->phkObject)->velocity[axis - 'X'] = velocity;
			}
		}
	}
	return true;
}

bool Cmd_GetLightFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH* light;
	UInt32 flagID;
	if (ExtractArgs(EXTRACT_ARGS, &light, &flagID) && (flagID <= 31) && (light->lightFlags & (1 << flagID))) {
		*result = 1;
	}
	return true;
}

bool Cmd_SetLightFlag_Execute(COMMAND_ARGS)
{
	TESObjectLIGH* light;
	UInt32 flagID, val;
	if (ExtractArgs(EXTRACT_ARGS, &light, &flagID, &val) && (flagID <= 31))
	{
		flagID = 1 << flagID;
		if (val) light->lightFlags |= flagID;
		else light->lightFlags &= ~flagID;
	}
	return true;
}

bool Cmd_GetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH* light;
	UInt32 traitID;
	if (ExtractArgs(EXTRACT_ARGS, &light, &traitID)) {
		switch (traitID)
		{
		case 0:
			*result = (int)light->radius;
			break;
		case 1:
			*result = light->red;
			break;
		case 2:
			*result = light->green;
			break;
		case 3:
			*result = light->blue;
			break;
		case 4:
			*result = light->falloffExp;
			break;
		case 5:
			*result = light->FOV;
			break;
		case 6:
			*result = light->fadeValue;
			break;
		default:
			break;
		}
	}
	return true;
}

bool Cmd_SetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	TESObjectLIGH* light;
	UInt32 traitID;
	float val;
	if (ExtractArgs(EXTRACT_ARGS, &light, &traitID, &val))
	{
		UInt32 intVal = (int)val;
		switch (traitID)
		{
		case 0:
			light->radius = intVal;
			break;
		case 1:
			light->red = intVal;
			break;
		case 2:
			light->green = intVal;
			break;
		case 3:
			light->blue = intVal;
			break;
		case 4:
			light->falloffExp = val;
			return true;
		case 5:
			light->FOV = val;
			return true;
		case 6:
			light->fadeValue = val;
			break;
		default:
			return true;
		}
	}
	return true;
}

bool Cmd_SetSystemColor_Execute(COMMAND_ARGS)
{
	UInt32 type, red, green, blue;
	if (ExtractArgs(EXTRACT_ARGS, &type, &red, &green, &blue) && type && (type <= 5))
		if (auto sysColor = SystemColorManager::GetSingleton()->sysColors.Head()->Advance(type - 1)->data)
		{
			sysColor->SetColorRGB(red, green, blue);
			ThisCall(0xBEF410, InterfaceManager::GetSingleton()->menuRoot, type, 0);
		}
	return true;
}

bool Cmd_GetSystemColor_Execute(COMMAND_ARGS) {
	*result = 0;
	UInt32 type;
	if (ExtractArgs(EXTRACT_ARGS, &type) && type > 0 && type <= 5) {
		SystemColorManager* colorMgr = SystemColorManager::GetSingleton();
		UInt32 color = (colorMgr->GetColor(type) >> 0x8);
		*result = color;
		if (IsConsoleMode()) Console_Print("GetSystemColor %d >> 0x%X", type, color);
	}
	return true;
}

bool Cmd_GetPipBoyMode_Execute(COMMAND_ARGS) {
	*result = InterfaceManager::GetSingleton()->pipBoyMode;
	if (IsConsoleMode()) Console_Print("GetPipBoyMode >> %.f", *result);
	return true;
}

bool Cmd_GetCollisionObjProperty_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 propID;
	if (ExtractArgs(EXTRACT_ARGS, &blockName, &propID) && (propID <= 8))
		if (hkpRigidBody* rigidBody = thisObj->GetRigidBody(blockName))
		{
			switch (propID)
			{
			case 0:
				*result = rigidBody->m_material.m_friction;
				break;
			case 1:
				*result = rigidBody->m_material.m_restitution;
				break;
			case 2:
				*result = rigidBody->m_motion.m_motionState.linearDamping;
				break;
			case 3:
				*result = rigidBody->m_motion.m_motionState.angularDamping;
				break;
			case 4:
			case 5:
			case 6:
				*result = rigidBody->m_motion.m_inertiaAndMassInv[propID - 4];
				break;
			case 7:
				if (rigidBody->m_motion.m_inertiaAndMassInv[3] > 0)
					*result = 1.0 / rigidBody->m_motion.m_inertiaAndMassInv[3];
				break;
			case 8:
				*result = rigidBody->m_motion.m_type;
				break;
			default:
				break;
			}
		}
	return true;
}



static bool PlayingSoundsIterator(TESSound* soundForm, bool doStop, TESObjectREFR* sourceRef, float fadeOutTime)
{
	BSAudioManager* audioMngr = BSAudioManager::Get();
	BSGameSound* gameSound;
	char soundPath[250] = "data\\sound\\";
	strcat(soundPath, soundForm->soundFile.path.m_data);

	if (sourceRef)
	{
		if (!sourceRef->loadedData || !sourceRef->loadedData->rootNode)
			return false;
		auto playingObjMap = &audioMngr->soundPlayingObjects;
		NiAVObject* soundObj;
		for (auto sndIter = audioMngr->playingSounds.Begin(); !sndIter.Done(); sndIter.Next())
		{
			gameSound = sndIter.Get();
			if (!gameSound || strcmp(soundPath, gameSound->filePath) != 0) {
				continue;
			}
			soundObj = playingObjMap->Lookup(sndIter.Key());
			if (!soundObj) {
				continue;
			}
			if (sourceRef->refID == 0x14) { // player
				NiNode* node = IS_TYPE(soundObj, BSFadeNode) ? (BSFadeNode*)soundObj : (NiNode*)soundObj;
				NiAVObject* root = node->GetBip01();
				if (!root || (root->m_flags & NiAVObject::kNiFlag_PlayerBone) == 0) {
					continue;
				}

			}
			else {
				TESObjectREFR* parentRef = soundObj->GetParentRef();
				if (!parentRef || parentRef != sourceRef) {
					continue;
				}
			}
			if (!doStop) return true;
			if (fadeOutTime == -1) {
				gameSound->stateFlags &= 0xFFFFFF0F;
				gameSound->stateFlags |= 0x10;
			}
			else {
				int time = fadeOutTime * 1000.0;
				ThisCall<void>(0xBD3020, BSAudioManager::Get(), gameSound->mapKey, time, 0x26); // BSAudioManager::StopSound_FadeOutTime
			}
		}
	}
	else
	{
		for (auto sndIter = audioMngr->playingSounds.Begin(); !sndIter.Done(); sndIter.Next())
		{
			if (!(gameSound = sndIter.Get()) || strcmp(soundPath, gameSound->filePath) != 0)
				continue;
			if (!doStop) return true;
			gameSound->stateFlags &= 0xFFFFFF0F;
			gameSound->stateFlags |= 0x10;
		}
	}
	return false;
}

bool Cmd_IsSoundPlaying_Execute(COMMAND_ARGS)
{
	TESSound* soundForm;
	TESObjectREFR* sourceRef = nullptr;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &soundForm, &sourceRef) && PlayingSoundsIterator(soundForm, false, sourceRef, -1)) {
		*result = 1;
	}
	if (IsConsoleMode()) {
		Console_Print("IsSoundPlaying >> %.f", *result);
	}
	return true;
}

bool Cmd_StopSoundAlt_Execute(COMMAND_ARGS)
{
	TESSound* soundForm;
	TESObjectREFR* sourceRef = nullptr;
	float fadeOutTime = -1;
	if (ExtractArgs(EXTRACT_ARGS, &soundForm, &sourceRef, &fadeOutTime))
		PlayingSoundsIterator(soundForm, true, sourceRef, fadeOutTime);
	return true;
}

bool Cmd_StopSound_Execute(COMMAND_ARGS)
{
	TESSound* soundForm;
	if (ExtractArgs(EXTRACT_ARGS, &soundForm)) {
		PlayingSoundsIterator(soundForm, true, nullptr, -1);
	}
	return true;
}

bool Cmd_RefreshIdle_Execute(COMMAND_ARGS) {
	*result = 0;
	UInt32 stopAnim = 0;
	Actor* actor = (Actor*)thisObj;
	ExtractArgs(EXTRACT_ARGS, &stopAnim);
	if (actor->baseProcess->GetForcedIdleForm()) {
		actor->baseProcess->ResetQueuedIdleFlags();
		actor->baseProcess->SetForcedIdleForm(NULL);
		if (stopAnim > 0) ThisCall<void>(0x460090, actor->GetAnimData(), 1, 1); // SpecialIdleFree
		*result = 1;
	}
	return true;
}
bool Cmd_IsDLLLoaded_Execute(COMMAND_ARGS) {
	*result = 0;
	int checkOutsideOfGameFolder = 0;
	char dllName[MAX_PATH];
	if (ExtractArgs(EXTRACT_ARGS, &dllName, &checkOutsideOfGameFolder)) {
		strncat(dllName, ".dll", 4);
		HMODULE module = GetModuleHandle(dllName);
		if (module) {
			if (!checkOutsideOfGameFolder) {
				char dllPath[MAX_PATH];
				char fnvPath[MAX_PATH];
				GetModuleFileNameA(module, dllPath, MAX_PATH);
				GetModuleFileNameA(NULL, fnvPath, MAX_PATH);
				fnvPath[strlen(fnvPath) - 13] = '\0';
				if (strstr(dllPath, fnvPath) != NULL) *result = 1;
			}
			else {
				*result = 1;
			}
		}
		if (IsConsoleMode()) Console_Print("IsDLLLoaded \"%s\" >> %.f", dllName, *result);
	}
	return true;
}

bool Cmd_GetActorVelocity_Execute(COMMAND_ARGS)
{
	char axis = 0;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &axis)) {
		if (bhkCharacterController* charCtrl = thisObj->GetCharacterController()) {
			if (axis) {
				*result = charCtrl->velocity[axis - 'X'];
			}
			else {
				__m128 vec = charCtrl->velocity.PS();
				__m128 sq = _mm_mul_ps(vec, vec);
				sq = _mm_hadd_ps(sq, sq);
				sq = _mm_hadd_ps(sq, sq);
				*result = sqrt(_mm_cvtss_f32(sq));
			}
			if (IsConsoleMode()) {
				Console_Print("GetActorVelocity >> %.5f", *result);
			}
		}
	}
	return true;
}

bool Cmd_GetIsRagdolled_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor() && (((Actor*)thisObj)->GetKnockedState() == 1))
		*result = 1;
	return true;
}

bool Cmd_GetKillXP_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor()) {
		PlayerCharacter* player = PlayerCharacter::GetSingleton();
		Actor* actor = (Actor*)thisObj;
		int level = ThisCall<int>(0x6F4CD0, actor);
		int typeReward = CdeclCall<int>(0x5A3A70, actor->baseForm->typeID != kFormType_Creature ? 1 : 0, level);
		double difficultyMult = CdeclCall<double>(0x585650, (float)typeReward, player->gameDifficulty);
		*result = floor(difficultyMult);
		if (IsConsoleMode()) {
			Console_Print("GetKillXP >> %.f", *result);
		}
	}
	return true;
}

bool Cmd_ResetFallTime_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor()) {
		BaseProcess* proc = ((Actor*)thisObj)->baseProcess;
		if (proc->uiProcessLevel <= 1) {
			bhkCharacterController* charCtrl = proc->GetCharacterController();
			if (charCtrl) {
				charCtrl->fallStartHeight = thisObj->posZ;
				charCtrl->fallTime = 0;
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_ClearMessageQueue_Execute(COMMAND_ARGS) {
	HUDMainMenu::GetSingleton()->ClearMessageQueue();
	*result = 1;
	return true;
}


bool Cmd_SetHotkeyItem_Execute(COMMAND_ARGS)
{
	*result = 0;
	int hotkeynum;
	TESForm* itemform = nullptr;

	if (!ExtractArgs(EXTRACT_ARGS, &hotkeynum, &itemform))
		return true;
	if (--hotkeynum < 8)
	{
		BSExtraData* xData = PlayerCharacter::GetSingleton()->extraDataList.GetByType(kExtraData_ContainerChanges);
		ExtraContainerChanges* xChanges = (ExtraContainerChanges*)xData;
		if (xChanges)
		{
			ExtraDataList* found = nullptr;
			ExtraHotkey* xHotkey = nullptr;
			// Remove the hotkey if it exists on another object.
			for (ExtraContainerChanges::EntryDataList::Iterator itemIter = xChanges->data->objList->Begin(); !itemIter.End(); ++itemIter)
			{
				if (itemIter->type->refID != itemform->refID)
				{
					for (ExtraContainerChanges::ExtendDataList::Iterator iter = itemIter->extendData->Begin(); !iter.End(); ++iter)
					{
						xHotkey = (ExtraHotkey*)iter->GetByType(kExtraData_Hotkey);
						if (xHotkey && xHotkey->index == hotkeynum)
						{
							found = iter.Get();
							break;
						}
					}
					if (found)
						break;
				}
			}
			if (found) {
				found->RemoveByType(kExtraData_Hotkey);
				found = nullptr;
			}

			xHotkey = nullptr;
			for (ExtraContainerChanges::EntryDataList::Iterator itemIter = xChanges->data->objList->Begin(); !itemIter.End(); ++itemIter)
			{
				if (itemIter->type && itemIter->type->refID == itemform->refID)
				{
					for (ExtraContainerChanges::ExtendDataList::Iterator iter = itemIter->extendData->Begin(); !iter.End(); ++iter)
					{
						xHotkey = (ExtraHotkey*)iter->GetByType(kExtraData_Hotkey);
						if (xHotkey) {
							xHotkey->index = hotkeynum; // If the item already has a hotkey, just change the index.
							break;
						}
					}
					if (!xHotkey)
					{
						ExtraHotkey* zHotkey = ExtraHotkey::Create();
						if (zHotkey)
						{
							zHotkey->index = hotkeynum;
							if (!itemIter->extendData)
								itemIter.Get()->Add(ExtraDataList::Create());
							if (itemIter->extendData)
							{
								if (!itemIter->extendData->Count())
									itemIter->extendData->AddAt(ExtraDataList::Create(), 0);
								if (itemIter->extendData->Count())
									itemIter->extendData->GetNthItem(0)->Add(zHotkey);
							}
						}
					}
					break;
				}
			}
		}
	}
	return true;
}

bool Cmd_ClearHotkey_Execute(COMMAND_ARGS)
{
	*result = 0;
	int hotkeynum;
	if (!ExtractArgs(EXTRACT_ARGS, &hotkeynum))
		return true;
	if (--hotkeynum < 8)
	{
		BSExtraData* xData = PlayerCharacter::GetSingleton()->extraDataList.GetByType(kExtraData_ContainerChanges);
		ExtraContainerChanges* xChanges = (ExtraContainerChanges*)xData;
		if (xChanges)
		{
			ExtraDataList* found = nullptr;
			ExtraHotkey* xHotkey = nullptr;
			for (ExtraContainerChanges::EntryDataList::Iterator itemIter = xChanges->data->objList->Begin(); !itemIter.End(); ++itemIter)
			{
				for (ExtraContainerChanges::ExtendDataList::Iterator iter = itemIter->extendData->Begin(); !iter.End(); ++iter)
				{
					xHotkey = (ExtraHotkey*)iter->GetByType(kExtraData_Hotkey);
					if (xHotkey && xHotkey->index == hotkeynum)
					{
						found = iter.Get();
						break;
					}
				}
				if (found)
					break;
			}
			if (found)
				found->RemoveByType(kExtraData_Hotkey);
		}
	}
	return true;
}

bool Cmd_PatchFreezeTime_Execute(COMMAND_ARGS) {
	if (!timePatched) {
		timePatched = true;
		SafeWrite16(0x6E48A6, 0x9090);
	}
	return true;
}



static void SetMapMarkerIcon(TESObjectREFR* marker, char* iconPath) {
	auto pos = markerIconMap.find(marker->refID);
	char* pathCopy = new char[strlen(iconPath) + 1];
	strcpy(pathCopy, iconPath);

	if (pos != markerIconMap.end()) {
		delete[] pos->second;
		pos->second = pathCopy;
	}
	else {
		markerIconMap.insert({ marker->refID, pathCopy });
	}
}

bool Cmd_SetCustomMapMarkerIcon_Execute(COMMAND_ARGS) {
	TESObjectREFR* form;
	char iconPath[MAX_PATH];
	if (!ExtractArgs(EXTRACT_ARGS, &form, &iconPath) || (!IS_TYPE(form, BGSListForm) && (!form->GetIsReference() || form->baseForm->refID != 0x10 || !GetExtraType(form->extraDataList, MapMarker)))) return true;
	if (IS_TYPE(form, BGSListForm)) {
		ListNode<TESForm>* iterator = ((BGSListForm*)form)->list.Head();
		while (iterator) {
			TESObjectREFR* ref = (TESObjectREFR*)(iterator->data);
			if (ref->GetIsReference() && ref->baseForm->refID == 0x10 && GetExtraType(ref->extraDataList, MapMarker)) {
				SetMapMarkerIcon(ref, iconPath);
			}
			iterator = iterator->next;
		}
	}
	else {
		SetMapMarkerIcon(form, iconPath);
	}
	if (IsConsoleMode()) Console_Print("SetCustomMapMarkerIcon >> %u, %s", form->refID, iconPath);
	return true;
}


bool Cmd_SetBipedIconPathAlt_Execute(COMMAND_ARGS) {
	UInt32 isFemale = 0;
	TESForm* form = NULL;
	char newPath[MAX_PATH];
	*result = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &newPath, &isFemale, &form)) {
		TESBipedModelForm* bipedModel = DYNAMIC_CAST(form, TESForm, TESBipedModelForm);
		if (bipedModel) {
			bipedModel->icon[isFemale].ddsPath.Set(newPath);
			*result = 1;
		}
	}

	return true;
}


bool Cmd_SetLightingTemplateTraitNumeric_Execute(COMMAND_ARGS) {
	*result = 0;
	UInt32 traitID = 0;
	BGSLightingTemplate* tmpl = nullptr;
	float value = 0.0;
	if (ExtractArgs(EXTRACT_ARGS, &tmpl, &traitID, &value) && IS_TYPE(tmpl, BGSLightingTemplate) && traitID > 0) {
		switch (traitID) {
		case 1:
		case 2:
		case 3:
			tmpl->data.ambientRGB[traitID - 1] = value;
			break;
		case 4:
		case 5:
		case 6:
			tmpl->data.directionalRGB[traitID - 4] = value;
			break;
		case 7:
		case 8:
		case 9:
			tmpl->data.fogRGB[traitID - 7] = value;
			break;
		case 10:
			tmpl->data.fogNear = value;
			break;
		case 11:
			tmpl->data.fogFar = value;
			break;
		case 12:
			tmpl->data.directionalXY = value;
			break;
		case 13:
			tmpl->data.directionalZ = value;
			break;
		case 14:
			tmpl->data.directionalFade = value;
			break;
		case 15:
			tmpl->data.fogClipDist = value;
			break;
		case 16:
			tmpl->data.fogPower = value;
			break;
		default:
			return true;
		}
	}
	return true;
}

bool Cmd_GetLightingTemplateTraitNumeric_Execute(COMMAND_ARGS) {
	*result = 0;
	UInt32 traitID = 0;
	BGSLightingTemplate* tmpl = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &tmpl, &traitID) && IS_TYPE(tmpl, BGSLightingTemplate) && traitID > 0) {
		switch (traitID) {
		case 1:
		case 2:
		case 3:
			*result = tmpl->data.ambientRGB[traitID - 1];
			break;
		case 4:
		case 5:
		case 6:
			*result = tmpl->data.directionalRGB[traitID - 4];
			break;
		case 7:
		case 8:
		case 9:
			*result = tmpl->data.fogRGB[traitID - 7];
			break;
		case 10:
			*result = tmpl->data.fogNear;
			break;
		case 11:
			*result = tmpl->data.fogFar;
			break;
		case 12:
			*result = tmpl->data.directionalXY;
			break;
		case 13:
			*result = tmpl->data.directionalZ;
			break;
		case 14:
			*result = tmpl->data.directionalFade;
			break;
		case 15:
			*result = tmpl->data.fogClipDist;
			break;
		case 16:
			*result = tmpl->data.fogPower;
			break;
		default:
			return true;
		}
		if (IsConsoleMode()) Console_Print("GetLightingTemplateTraitNumeric %d >> %f", traitID, *result);
	}
	return true;
}


bool Cmd_IsLoadDoor_Execute(COMMAND_ARGS) {
	*result = 0;

	if (GetByTypeCast(thisObj->extraDataList, Teleport) || GetByTypeCast(thisObj->extraDataList, RandomTeleportMarker)) {
		*result = 1;
	}
	if (IsConsoleMode()) {
		Console_Print("IsLoadDoor >> %.f", *result);
	}
	return true;
}

static bool IsUnlockedOrHacked(TESObjectREFR* obj) {
	BGSTerminal* terminal = (BGSTerminal*)obj->baseForm;
	bool isUnlocked = (terminal->data.terminalFlags & 2) != 0;
	ExtraTerminalState* xState = (ExtraTerminalState*)obj->extraDataList.GetByType(kExtraData_TerminalState);
	if (xState) {
		if ((xState->lockedOut & 0x80) != 0 || xState->lockLevel == -2 || xState->lockLevel == -1) {
			return true;
		}
		else {
			return false;
		}
	}
	return isUnlocked;
}
bool Cmd_GetLockedAlt_Execute(COMMAND_ARGS) {
	*result = 0;
	if (thisObj->baseForm->typeID == kFormType_Terminal) {
		if (ThisCall<UInt32>(0x774660, PlayerCharacter::GetSingleton(), thisObj) == 2) {
			*result = 2;
		}
		else if (!IsUnlockedOrHacked(thisObj)) {
			*result = 1;
		}
	}
	if (IsConsoleMode()) {
		Console_Print("GetLockedAlt >> %.f", *result);
	}
	return true;
}


bool Cmd_GetCrosshairRefEx_Execute(COMMAND_ARGS) {
	*result = 0;
	TESObjectREFR* ref = HUDMainMenu::GetSingleton()->crosshairRef;
	if (ref) {
		*(UInt32*)result = ref->refID;
	}
	else {
		float fDistance = 0;
		bool bResult = false;
		PlayerCharacter* player = PlayerCharacter::GetSingleton();
		NiNode* g_camera1st = *(NiNode**)0x107BA78;
		NiPoint3 pos(player->kCamera1stPos);
		if (!g_camera1st) {
			pos = player->GetPos();
			double scaledHeight = ThisCall<double>(0x4E6D80, player);
			pos.z = scaledHeight * player->eyeHeight + pos.z;
		}
		if (player->bThirdPerson) {
			pos += player->camera3rdPos;
		}
		NiMatrix33 zMatrix = NiMatrix33();
		zMatrix.MakeZRotation(player->AdjustRot(0));
		NiMatrix33 xMatrix = NiMatrix33();
		xMatrix.MakeXRotation(player->rotX);

		NiMatrix33 outMatrix = zMatrix.TransposeTimes(xMatrix);
		NiPoint3 rot = outMatrix.GetCol(1);

		InterfaceManager* g_interfaceManager = InterfaceManager::GetSingleton();
		ref = ThisCall<TESObjectREFR*>(0x574540, g_interfaceManager->viewCaster, &pos, &rot, 0x46400000, &fDistance, &bResult);
		if (ref) {
			*(UInt32*)result = ref->refID;
		}
	}
	if (IsConsoleMode()) {
		Console_Print("GetCrosshairRefEx >> 0x%X", ref->refID);
	}
	return true;
}

bool Cmd_SetTexturePath_Execute(COMMAND_ARGS)
{
	*result = 0;
	char path[MAX_PATH];
	TESForm* form = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &path, &form)) {
		if (IS_TYPE(form, TESObjectREFR)) {
			form = ((TESObjectREFR*)form)->baseForm;
		}
		TESTexture* tex = DYNAMIC_CAST(form, TESForm, TESTexture);
		if (tex) {
			tex->ddsPath.Set(path);
			*result = 1;

		}
		else {
			TESModelTextureSwap* texs = DYNAMIC_CAST(form, TESForm, TESModelTextureSwap);
			if (texs)
			{
				texs->SetPath(path);
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_GetKiller_Execute(COMMAND_ARGS) {
	Actor* actor = (Actor*)thisObj;
	*result = 0;
	if (actor->IsActor() && (actor->lifeState == 1 || actor->lifeState == 2)) {
		TESObjectREFR* killer = actor->killer;
		if (killer != nullptr) {
			*(UInt32*)result = killer->refID;
			if (IsConsoleMode()) {
				Console_Print("GetKiller >> 0x%X", killer->refID);
			}
		}
	}

	return true;
}

bool Cmd_IsKeyPressedAlt_Execute(COMMAND_ARGS) {
	int keyCode;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &keyCode)) {
		if (keyCode < 255)
		{
			keyCode = ScancodeToVirtualKey(keyCode);
			if (keyCode != NOKEY) *result = (GetAsyncKeyState(keyCode) & 0x8000) ? true : false;
		}
		else {
			cmd_IsKeyPressed->eval(thisObj, (void*)keyCode, nullptr, result);
		}
	}
	return true;
}
bool Cmd_MessageBoxEx_Execute(COMMAND_ARGS) {
	*result = 0;
	if (!ExtractFormatStringArgs(0, s_strValBuffer, EXTRACT_ARGS_EX, kCommandInfo_MessageBoxEx.numParams))
		return true;

	//extract the buttons
	const char* b[10] = { nullptr };
	UInt32 btnIdx = 0;

	for (char* ch = s_strValBuffer; *ch && btnIdx < 10; ch++)
	{
		if (*ch == '|')
		{
			*ch = '\0';
			b[btnIdx++] = ch + 1;
		}
	}

	if (!btnIdx)
		b[0] = "Ok";

	if (thisObj && !(thisObj->flags & TESForm::kFormFlags_DontSaveForm))		// if not temporary object and not quest script
		*(UInt32*)0x1071B80 = thisObj->refID;
	else
		*(UInt32*)0x1071B80 = scriptObj->refID;

	*(UInt8*)0xF50B78 = 0xFF;	// overwrite any previously pressed button
	CdeclCall<void>(0x619700, s_strValBuffer, 0, 0, 0x51E270, 0, 0x17, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], NULL);

	return true;
}

bool Cmd_MessageExAlt_Execute(COMMAND_ARGS) {
	*result = 0;

	float displayTime;
	if (!ExtractFormatStringArgs(1, s_strValBuffer, EXTRACT_ARGS_EX, kCommandInfo_MessageExAlt.numParams, &displayTime)) return true;
	const char* msgIcon = nullptr;
	const char* msgSound = nullptr;
	while (true)
	{
		char* barPtr = GetNextToken(s_strValBuffer, '|');
		if (!*barPtr) break;
		if (*s_strValBuffer == '$')
			msgSound = s_strValBuffer + 1;
		else if (*s_strValBuffer == '#')
		{
			char iconIdx = s_strValBuffer[1] - '0';
			if ((iconIdx >= 0) && (iconIdx <= 6))
				msgIcon = (const char*)kMsgIconsPathAddr[iconIdx];
		}
		else msgIcon = s_strValBuffer;
		s_strValBuffer = barPtr;
	}
	s_strValBuffer[0x203] = 0;
	CdeclCall<void>(0x61B850, s_strValBuffer, 0, msgIcon, msgSound, displayTime);
	return true;
}

static TESForm* GetOwner(BaseExtraList* xDataList)
{
	return ThisCall<TESForm*>(0x40ABC0, xDataList);
}

static TESForm* GetCellOwner(TESObjectCELL* cell) {
	return ThisCall<TESForm*>(0x4DAAE0, cell);
}

static SInt32 GetRequiredRank(BaseExtraList* xDataList) {
	return ThisCall<SInt32>(0x40AC00, xDataList);
}

static SInt32 GetFactionRank(Actor* actor, TESFaction* faction) {
	return ThisCall<SInt32>(0x44F6A0, &actor->GetActorBase()->baseData, faction, actor == PlayerCharacter::GetSingleton());
}

bool Cmd_IsOwned_Execute(COMMAND_ARGS) {
	*result = 0;
	Actor* actor = NULL;
	if (!thisObj) return true;

	if (ExtractArgs(EXTRACT_ARGS, &actor)) {

		if (!actor) return true;

		SInt32 requiredRank = 0;

		TESForm* owner = GetOwner(&thisObj->extraDataList);

		if (owner) {
			requiredRank = GetRequiredRank(&thisObj->extraDataList);
			if (requiredRank == -1) requiredRank = 0;

		}
		else {
			owner = GetCellOwner(thisObj->parentCell);
			if (owner) {
				requiredRank = GetRequiredRank(&thisObj->parentCell->extraDataList);
				if (requiredRank == -1) requiredRank = 0;
			}
		}

		if (owner) {
			if (owner->refID == actor->baseForm->refID) {
				*result = 1.0;
			}
			else {
				TESFaction* faction = DYNAMIC_CAST(owner, TESForm, TESFaction);
				if (faction) {
					*result = (GetFactionRank(actor, faction) >= requiredRank) ? 1.0 : 0.0;
				}
			}
		}

	}
	if (IsConsoleMode()) {
		Console_Print("IsOwned >> %.f", *result);
	}
	return true;
}

bool Cmd_AddItemOwnership_Execute(COMMAND_ARGS) {
	*result = 0;
	TESForm* pItem = nullptr;
	float NumItems = 1;
	TESForm* pOwner = nullptr;
	UInt32 Rank = 0;

	if (ExtractArgs(EXTRACT_ARGS, &pItem, &NumItems, &pOwner, &Rank)) {
		if (!thisObj) return true;

		TESScriptableForm* pScript = DYNAMIC_CAST(pItem, TESForm, TESScriptableForm);
		if (pScript && !pScript->script) pScript = nullptr;  // Only existing scripts matter


		ExtraOwnership* pXOwner = nullptr;
		ExtraRank* pXRank = nullptr;
		ExtraDataList* pExtraDataList = nullptr;
		ExtraScript* pXScript = nullptr;

		if (pOwner || Rank || pScript) {
			pExtraDataList = ExtraDataList::Create();
			if (pOwner) {
				pXOwner = ExtraOwnership::Create();
				pExtraDataList->Add(pXOwner);
				pXOwner->owner = pOwner;
			}
			if (Rank) {
				pXRank = ExtraRank::Create();
				pExtraDataList->Add(pXRank);
				pXRank->rank = Rank;
			}
			if (pScript) {
				pXScript = ExtraScript::Create(pItem, true);
				pExtraDataList->Add(pXScript);
			}
		}
		thisObj->AddItem(pItem, pExtraDataList, NumItems);
		*result = 1;
	}
	return true;
}

bool Cmd_GetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	TESWorldSpace* wspc;
	UInt32 flagID;
	if (ExtractArgs(EXTRACT_ARGS, &wspc, &flagID) && (flagID <= 7) && (wspc->flags & (1 << flagID)))
		*result = 1;
	if (IsConsoleMode()) {
		Console_Print("GetWorldspaceFlag %d >> %.f", flagID, *result);
	}
	return true;
}

bool Cmd_SetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	TESWorldSpace* wspc;
	UInt32 flagID, val;
	if (ExtractArgs(EXTRACT_ARGS, &wspc, &flagID, &val) && (flagID <= 7))
	{
		flagID = 1 << flagID;
		if (val) wspc->flags |= flagID;
		else wspc->flags &= ~flagID;
	}
	return true;
}

bool Cmd_GetPCCanFastTravel_Execute(COMMAND_ARGS)
{
	// Credits to Jazz for the "silence QueueUIMessage" trick (see AddNoteNS).
	SafeWrite8((UInt32)0x61B850, 0xC3);	// RETN
	const auto canFastTravelAddr = GetRelJumpAddr(0x6680D3); // call the function indirectly for compatibility with Stewie tweaks, kudos to Stewie.
	*result = ThisCall<bool>(canFastTravelAddr, PlayerCharacter::GetSingleton());
	SafeWrite16((UInt32)0x61B850, 0xFF6A);	// PUSH 0FFFFFFFFh
	if (IsConsoleMode()) {
		Console_Print("GetPCCanFastTravel >> %.f", *result);
	}
	return true;
}

static float GetRadiationLevel(Actor* actor, bool scaleByResist)
{
	if (actor->IsActor()) {
		if (HighProcess* hiProc = (HighProcess*)actor->baseProcess; hiProc && !hiProc->uiProcessLevel) {
			if (actor == PlayerCharacter::GetSingleton()) {
				return hiProc->fWaterRadsSec + hiProc->rads238 + hiProc->GetRadsSec();
			}
			else {
				return actor->GetRadiationLevel(scaleByResist);
			}
		}
	}
	return true;
}

bool Cmd_GetRadiationLevelAlt_Execute(COMMAND_ARGS) {
	*result = GetRadiationLevel((Actor*)thisObj, true);
	if (IsConsoleMode()) {
		Console_Print("GetRadiationLevelAlt >> %.2f", *result);
	}
	return true;
}


bool Hook_GetRadiationLevel_Eval(COMMAND_ARGS_EVAL) {
	if (thisObj->IsActor() && thisObj != PlayerCharacter::GetSingleton()) {
		*result = GetRadiationLevel((Actor*)thisObj, false);
		return true;
	}
	else {
		CdeclCall<bool>(0x513C30, thisObj, 0, 0, result);
		return true;
	}
}

bool Hook_GetRadiationLevel_Execute(COMMAND_ARGS) {
	Hook_GetRadiationLevel_Eval(thisObj, 0, 0, result);
	return true;
}

bool Hook_ListAddReference_Execute(COMMAND_ARGS)
{
	*result = eListInvalid;
	BGSListForm* pListForm = NULL;
	UInt32 n = eListEnd;

	if (ExtractArgs(EXTRACT_ARGS, &pListForm, &n)) {
		if (!pListForm || !thisObj) return true;

		UInt32 index = pListForm->AddAt(thisObj, n);
		if (index != eListInvalid) {
			*result = index;
		}
		if (IsConsoleMode()) {
			Console_Print("Index: %d", index);
		}
	}
	return true;
}

bool Hook_ListAddForm_Execute(COMMAND_ARGS)
{
	*result = eListInvalid;
	BGSListForm* pListForm = NULL;
	TESForm* pForm = NULL;
	UInt32 n = eListEnd;

	ExtractArgsEx(EXTRACT_ARGS_EX, &pListForm, &pForm, &n);
	if (pListForm && pForm) {
		UInt32 index = pListForm->AddAt(pForm, n);
		if (index != eListInvalid) {
			*result = index;
		}
		if (IsConsoleMode()) {
			Console_Print("Index: %d", index);
		}
	}

	return true;
}

bool Hook_GetHotkeyItem_Execute(COMMAND_ARGS)
{
	UInt32* refResult = (UInt32*)result;
	*refResult = 0;

	UInt32 hotkeyNum;	// passed as 1 - 8, stored by game as 0-7
	if (ExtractArgs(EXTRACT_ARGS, &hotkeyNum) && --hotkeyNum < 8)
	{
		BSExtraData* xData = PlayerCharacter::GetSingleton()->extraDataList.GetByType(kExtraData_ContainerChanges);
		ExtraContainerChanges* xChanges = (ExtraContainerChanges*)xData;
		if (xChanges)
		{
			for (ExtraContainerChanges::EntryDataList::Iterator itemIter = xChanges->data->objList->Begin();
				!itemIter.End();
				++itemIter)
			{
				if (itemIter.Get()) {
					for (ExtraContainerChanges::ExtendDataList::Iterator iter = itemIter->extendData->Begin();
						!iter.End();
						++iter)
					{
						if (iter.Get()) {
							ExtraHotkey* xHotKey = (ExtraHotkey*)iter.Get()->GetByType(kExtraData_Hotkey);
							if (xHotKey && xHotKey->index == hotkeyNum)
							{
								*refResult = itemIter->type->refID;
								if (IsConsoleMode())
									Console_Print("GetHotkeyItem >> %08x (%s)", *refResult, GetFullName(itemIter->type));

								return true;
							}
						}
					}
				}
			}
		}
	}

	// not found
	if (IsConsoleMode())
		Console_Print("GetHotkeyItem >> Hotkey not assigned");

	return true;
}

bool Cmd_GetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	TESForm* object;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &object))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data)
			*result = (int)destructible->data->health;
	return true;
}

bool Cmd_SetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 value;
	if (ExtractArgs(EXTRACT_ARGS, &object, &value))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data)
			destructible->data->health = value;
	return true;
}

bool Cmd_GetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	TESForm* object;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &object))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data && destructible->data->targetable)
			*result = 1;
	return true;
}

bool Cmd_SetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 value;
	if (ExtractArgs(EXTRACT_ARGS, &object, &value))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data)
			destructible->data->targetable = value != 0;
	return true;
}

bool Cmd_GetDestructionDataNumStages_Execute(COMMAND_ARGS)
{
	TESForm* object;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &object))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data)
			*result = destructible->data->stageCount;
	return true;
}

bool Cmd_GetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 idx, traitID;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &traitID))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			switch (traitID)
			{
			case 0:
				*result = destructible->data->stages[idx]->healthPrc;
				break;
			case 1:
				*result = (int)destructible->data->stages[idx]->selfDmgSec;
				break;
			case 2:
				*result = destructible->data->stages[idx]->dmgStage;
				break;
			case 3:
				*result = destructible->data->stages[idx]->flags;
				break;
			case 4:
				*result = (int)destructible->data->stages[idx]->debrisCount;
				break;
			default:
				break;
			}
	return true;
}

bool Cmd_SetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 idx, traitID, value;
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &traitID, &value))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			switch (traitID)
			{
			case 0:
				destructible->data->stages[idx]->healthPrc = value % 101;
				break;
			case 1:
				destructible->data->stages[idx]->selfDmgSec = value;
				break;
			case 2:
				destructible->data->stages[idx]->dmgStage = value % 9;
				break;
			case 3:
				destructible->data->stages[idx]->flags = value & 7;
				break;
			case 4:
				destructible->data->stages[idx]->debrisCount = value;
				break;
			default:
				break;
			}
	return true;
}

bool Cmd_GetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 idx;
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data &&
			destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->explosion)
			*(UInt32*)result = destructible->data->stages[idx]->explosion->refID;
	return true;
}

bool Cmd_SetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 idx;
	BGSExplosion* explForm = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &explForm))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			destructible->data->stages[idx]->explosion = (explForm && (explForm->typeID == kFormType_Explosion)) ? explForm : nullptr;
	return true;
}

bool Cmd_GetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 idx;
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx))
		*result = 0;
	if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data &&
		destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->debris)
		*(UInt32*)result = destructible->data->stages[idx]->debris->refID;
	return true;
}

bool Cmd_SetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	TESForm* object;
	UInt32 idx;
	BGSDebris* dbrForm = nullptr;
	if (ExtractArgs(EXTRACT_ARGS, &object, &idx, &dbrForm))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			destructible->data->stages[idx]->debris = (dbrForm && (dbrForm->typeID == kFormType_Debris)) ? dbrForm : nullptr;
	return true;
}

bool Cmd_GetButcherPeteVersion_Execute(COMMAND_ARGS) {
	*result = g_version;
	if (IsConsoleMode()) {
		Console_Print("GetButcherPeteVersion >> %.f", *result);
	}
	return true;
}

bool Cmd_GetButcherPeteVersion_Eval(COMMAND_ARGS_EVAL) {
	*result = g_version;
	return true;
}

bool Cmd_SetTextureSetTexture_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSTextureSet* texSet = nullptr;
	int mapID = -1;
	char path[MAX_PATH];
	if (ExtractArgs(EXTRACT_ARGS, &texSet, &mapID, &path) && (mapID >= 0 && mapID <= 5)) {
		texSet->textures[mapID].ddsPath.Set(path);
		*result = 1;
	}
	return true;
};


bool Cmd_GetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage* message;
	if (ExtractArgs(EXTRACT_ARGS, &message)) {
		*result = message->msgFlags;
		if (IsConsoleMode()) {
			Console_Print("GetMessageFlags >> %.f", *result);
		}
	}

	return true;
}

bool Cmd_SetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage* message;
	UInt32 flags;
	if (ExtractArgs(EXTRACT_ARGS, &message, &flags) && (flags <= 3))
		message->msgFlags = flags;
	return true;
}

bool Cmd_SetMessageDisplayTime_Execute(COMMAND_ARGS)
{
	BGSMessage* message;
	UInt32 displayTime;
	if (ExtractArgs(EXTRACT_ARGS, &message, &displayTime))
		message->displayTime = displayTime;
	return true;
}