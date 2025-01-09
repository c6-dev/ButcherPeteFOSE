#pragma once
#include <unordered_map>
#include "havok.h"
#include "fn_destruction_data.h"
#include "GameSettings.h"
#define REG_CMD(name) fose->RegisterCommand(&kCommandInfo_##name);
#undef MessageBoxEx

DEFINE_COMMAND_PLUGIN(IsOwned, 1, kParams_OneActorRef);
DEFINE_COMMAND_PLUGIN(AddItemOwnership, 1, kParams_OneForm_OneFloat_OneForm_OneOptionalRank);
DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, 0, kParams_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, 0, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetPCCanFastTravel, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadiationLevelAlt, 1, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetButcherPeteVersion, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(MessageExAlt, 0,  kParams_OneFloat_OneFormatString);
DEFINE_COMMAND_PLUGIN(MessageBoxEx, 0, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(IsKeyPressedAlt, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetKiller, 1, NULL);
DEFINE_COMMAND_PLUGIN(SetTexturePath, 0, kParams_OneString_OneForm);
DEFINE_COMMAND_PLUGIN(GetCrosshairRefEx, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetLockedAlt, 1, NULL);
DEFINE_COMMAND_PLUGIN(IsLoadDoor, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetLightingTemplateTraitNumeric, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightingTemplateTraitNumeric, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(SetBipedIconPathAlt, 0, kParams_OneString_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetCustomMapMarkerIcon, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(PatchFreezeTime, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetHotkeyItem, 0, kParams_SetHotkeyItem);
DEFINE_COMMAND_PLUGIN(ClearHotkey, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ClearMessageQueue, 0, NULL);
DEFINE_COMMAND_PLUGIN(ResetFallTime, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetKillXP, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetIsRagdolled, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetActorVelocity, 1, kParams_OneOptionalAxis);
DEFINE_COMMAND_PLUGIN(IsDLLLoaded, 0, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RefreshIdle, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(StopSound, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(StopSoundAlt, 0, kParams_TwoForms_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(IsSoundPlaying, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCollisionObjProperty, 1, kParams_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(GetPipBoyMode, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetSystemColor, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetSystemColor, 0, kParams_FourInts);
DEFINE_COMMAND_PLUGIN(GetLightTraitNumeric, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightTraitNumeric, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetLightFlag, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightFlag, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetActorVelocity, 1, kParams_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(GetFallTimeElapsed, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetFallTimeRemaining, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetActorGravityMult, 1, NULL);
DEFINE_COMMAND_PLUGIN(SetActorGravityMult, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(IsInWater, 1, NULL);
DEFINE_COMMAND_PLUGIN(GetCreatureType, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(PlayIdleEx, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetPlayedIdle, 1, NULL);
DEFINE_COMMAND_PLUGIN(IsIdlePlayingEx, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetUIFloatGradual, 0, kParams_OneString_ThreeOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(AddTileFromTemplate, 0, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(MoveToCell, 1, kParams_OneForm_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetGameVolume, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetGameVolume, 0, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetClimateSunGlareTexture, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetClimateSunTexture, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateTraitNumeric, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetClimateTraitNumeric, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(RefreshCurrentClimate, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCurrentClimate, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetCurrentClimate, 0, kParams_OneForm);
int g_version = 220;

char* s_strArgBuffer;
char* s_strValBuffer;
std::unordered_map<UInt32, char*> markerIconMap;
const UInt32 kMsgIconsPathAddr[] = { 0xDC0C38, 0xDC0C78, 0xDC5544, 0xDCE658, 0xDD9148, 0xDE3790, 0xDF3278 };

FOSECommandTableInterface* cmdTableInterface = nullptr;
CommandInfo* cmd_IsKeyPressed = nullptr;

char** defaultMarkerList = (char**)0xF6B13C;

bool timePatched = false;

TESObjectREFR* s_tempPosMarker;

bool Cmd_GetCurrentClimate_Execute(COMMAND_ARGS)
{
	Sky* currentSky = Sky::Get();
	TESClimate* climate = currentSky->currClimate;
	if (climate)
		*(UInt32*)result = climate->refID;
	if (IsConsoleMode()) {
		Console_Print("GetCurrentClimate >> 0x%X %s", climate->refID, climate->GetEditorID());
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
	currentSky->RefreshClimate(currentSky->currClimate, true);
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
	for (char* p = current_pos; (current_pos = strchr(str, '/')) != NULL; *current_pos = '\\');
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
				} else {
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
	UInt8 color[3] = { 0, 0, 0 };
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
			}
		}
	return true;
}



bool PlayingSoundsIterator(TESSound* soundForm, bool doStop, TESObjectREFR* sourceRef, float fadeOutTime)
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
				
			} else {
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
	char dllPath[MAX_PATH];
	char fnvPath[MAX_PATH];
	if (ExtractArgs(EXTRACT_ARGS, &dllName, &checkOutsideOfGameFolder)) {
		strncat(dllName, ".dll", 4);
		HMODULE module = GetModuleHandle(dllName);
		if (module) {
			if (!checkOutsideOfGameFolder) {
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
	TESForm* itemform = NULL;
	ExtraDataList* found = NULL;
	ExtraHotkey* xHotkey = NULL;

	if (!ExtractArgs(EXTRACT_ARGS, &hotkeynum, &itemform))
		return true;
	if (--hotkeynum < 8)
	{
		BSExtraData* xData = PlayerCharacter::GetSingleton()->extraDataList.GetByType(kExtraData_ContainerChanges);
		ExtraContainerChanges* xChanges = (ExtraContainerChanges*)xData;
		if (xChanges)
		{
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
				};
			}
			if (found) {
				found->RemoveByType(kExtraData_Hotkey);
				found = NULL;
			}

			xHotkey = NULL;
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
	ExtraDataList* found = NULL;
	ExtraHotkey* xHotkey = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &hotkeynum))
		return true;
	if (--hotkeynum < 8)
	{
		BSExtraData* xData = PlayerCharacter::GetSingleton()->extraDataList.GetByType(kExtraData_ContainerChanges);
		ExtraContainerChanges* xChanges = (ExtraContainerChanges*)xData;
		if (xChanges)
		{
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
char* __fastcall GetMapMarker(TESObjectREFR* thisObj, UInt16 mapMarkerType) {
	auto it = markerIconMap.find(thisObj->refID);
	if (it != markerIconMap.end()) return it->second;
	return defaultMarkerList[mapMarkerType];
}


void SetMapMarkerIcon(TESObjectREFR* marker, char* iconPath) {
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

bool IsUnlockedOrHacked(TESObjectREFR* obj) {
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
	TESObjectREFR* killer = nullptr;
	if (actor->IsActor() && (actor->lifeState == 1 || actor->lifeState == 2)) {
		killer = actor->killer;
		*(UInt32*)result = killer->refID;
		if (IsConsoleMode()) {
			Console_Print("GetKiller >> 0x%X", killer->refID);
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
			if (!(keyCode == NOKEY)) *result = (GetAsyncKeyState(keyCode) & 0x8000) ? true : false;
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

TESForm* GetOwner(BaseExtraList* xDataList)
{
	return ThisCall<TESForm*>(0x40ABC0, xDataList);
}

TESForm* GetCellOwner(TESObjectCELL* cell) {
	return ThisCall<TESForm*>(0x4DAAE0, cell);
}

SInt32 GetRequiredRank(BaseExtraList* xDataList) {
	return ThisCall<SInt32>(0x40AC00, xDataList);
}

SInt32 GetFactionRank(Actor* actor, TESFaction* faction) {
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
	TESForm* pItem = NULL;
	float NumItems = 1;
	TESForm* pOwner = NULL;
	UInt32 Rank = 0;
	TESForm* rcItem = NULL;

	if (ExtractArgs(EXTRACT_ARGS, &pItem, &NumItems, &pOwner, &Rank)) {
		if (!thisObj) return NULL;

		TESScriptableForm* pScript = DYNAMIC_CAST(pItem, TESForm, TESScriptableForm);
		if (pScript && !pScript->script) pScript = NULL;  // Only existing scripts matter


		ExtraOwnership* pXOwner = NULL;
		ExtraRank* pXRank = NULL;
		ExtraDataList* pExtraDataList = NULL;
		ExtraScript* pXScript = NULL;

		if ( pOwner || Rank || pScript) {
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

float GetRadiationLevel(Actor* actor, bool scaleByResist)
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


_declspec(naked) void uGridsLoadingCrashHook()
{
	// add a null check after call to GridCellArray::GetCell before dereferencing it in SaveLoad::LoadGlobalBufferedCells
	_asm
	{
		test eax, eax
		jz done
		mov esi, dword ptr ds : [eax]
		done :
		lea edx, dword ptr ss : [esp + 0x1C]
		mov eax, 0x43773C
		jmp eax
	}
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
const char* GetPackageTypeName(char type) {
	switch (type) {
	case 0:
		return "Explore";
	case 1:
		return "Follow";
	case 2:
		return "Escort";
	case 3:
		return "Eat";
	case 4:
		return "Sleep";
	case 5:
		return "Wander";
	case 6:
		return "Travel";
	case 7:
		return "Accompany";
	case 8:
		return "Use Item At";
	case 9:
		return "Ambush";
	case 10:
		return "Flee Not Combat";
	case 11:
		return "Cast Magic";
	case 12:
		return "Sandbox";
	case 13:
		return "Patrol";
	case 14:
		return "Guard";
	case 15:
		return "Dialogue";
	case 16:
		return "Use Weapon";
	case 18:
		return "Combat";
	case 19:
		return "Combat Low";
	case 20:
		return "Activate";
	case 21:
		return "Alarm";
	case 22:
		return "Flee";
	case 23:
		return "Trespass";
	case 24:
		return "Spectator";
	case 25:
		return "React To Dead";
	case 26:
		return "Get Up";
	case 27:
		return "Do Nothing";
	case 28:
		return "In-Game Dialogue";
	case 29:
		return "Surface";
	case 30:
		return "Search For Attacker";
	case 31:
		return "Avoid Radiation";
	}
	return "";
}
TESPackage* __fastcall GetAIPackageHook(Actor* actor) {
	TESPackage* package = ThisCall<TESPackage*>(0x766020, actor);
	if (IsConsoleMode()) {
		Console_Print("Current Package: 0x%X (%s)\nPackage Type: %d (%s)", package->refID, package->GetEditorID(), package->type, GetPackageTypeName(package->type));
	}
	return package;
}

__declspec (naked) void GetMapMarkerHook() {
	//UInt32 static const retAddr = 0x079D337;
	__asm
	{
		mov edx, eax
		mov ecx, edi
		jmp GetMapMarker
	}
}
void __fastcall SetTreeFullLODToINISetting(TESObjectCELL* cell)
{
	auto mgr = BSTreeManager::GetSingleton(true);
	Setting* bForceFullLOD = (Setting*)0x1073C94;
	mgr->isForceFullLOD = bForceFullLOD->data.i;
	ThisCall<void>(0x4D8120, cell);
}
void MarkNode(NiNode* node) {
	if (node) {
		node->m_flags |= NiAVObject::kNiFlag_PlayerBone;
		if (node->IsNiNode()) {
			for (int i = 0; i < node->m_children.Length(); i++) {
				NiAVObject* node3 = node->m_children[i];
				MarkNode((NiNode*)node3);
			} 
		}
	}
}

void __stdcall MarkPlayerBones() {
	StdCall<void>(0x619EB0);
	PlayerCharacter* player = PlayerCharacter::GetSingleton();
	NiNode* firstPerson = player->pBipedAnims1st->pRoot;
	if (firstPerson) firstPerson->m_flags |= NiAVObject::kNiFlag_PlayerBone;
	NiNode* thirdPerson = player->bipedAnim->pRoot;
	if (thirdPerson) thirdPerson->m_flags |= NiAVObject::kNiFlag_PlayerBone;
	 
}

void WritePatches() {
	s_strArgBuffer = (char*)malloc(0x4000);
	s_strValBuffer = (char*)malloc(0x10000);

	WriteRelJump(0x437736, UInt32(uGridsLoadingCrashHook)); // fix crash when loading a save with increased ugrids after lowering them
	WriteRelJump(0x4FDD9F, 0x4FDDB9); // increase grass render distance
	WriteRelCall(0x5110A5, (UInt32)GetAIPackageHook);

	// SetCustomMapMarkerIcon
	SafeWrite16(0x6654DB, 0x9090); // nop 2b
	WriteRelCall(0x6654DD, (UInt32)GetMapMarkerHook); // call 5b
	SafeWrite8(0x6654E2, 0x50); // push eax instead of edx
	WriteRelCall(0x4DBE16, (UInt32)SetTreeFullLODToINISetting); // fixed bForceFullLOD resetting when opening pipboy (thanks Stewie)
	WriteRelCall(0x76FA6A, (UInt32)MarkPlayerBones);

}

void WriteEditorPatches()
{
	SafeWriteBuf(0xD4A838, "GetButcherPeteVersion\0", 22);
}