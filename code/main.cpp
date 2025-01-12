#include "PluginAPI.h"
#include "CommandTable.h"
#include "GameAPI.h"
#include "ParamInfos.h"
#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"
#include <string>
#include "SafeWrite.h"
#include "GameMenus.h"
#include "main.h"
#include "fose_version.h"

IDebugLog		gLog("butcher_pete.log");

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;


extern "C" {

bool FOSEPlugin_Query(const FOSEInterface * fose, PluginInfo * info)
{

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "Butcher Pete FOSE";
	info->version = g_version;
	// version checks
	if (fose->isNogore)
	{
		_ERROR("German No-Gore version is not supported");
		return false;
	}
	if(fose->foseVersion < 0x1030020)
	{
		MessageBoxA(nullptr, "ButcherPete FOSE plugin requires FOSE version v1.3b2.\n\nInstalled version is too old.", "ButcherPete FOSE", 0);
		_ERROR("FOSE 1.3b2 is required for ButcherPete FOSE. Your version is %08X", fose->foseVersion);
		return false;
	}

	if(!fose->isEditor && fose->runtimeVersion != FALLOUT_VERSION_1_7)
	{
		_ERROR("incorrect runtime version (got %08X need %08X)", fose->runtimeVersion, FALLOUT_VERSION_1_7);
		return false;
	}

	
	return true;
}


bool FOSEPlugin_Load(const FOSEInterface * fose)
{
	fose->SetOpcodeBase(0x2220);
	g_pluginHandle = fose->GetPluginHandle();
	REG_CMD(IsOwned);
	REG_CMD(AddItemOwnership);
	REG_CMD(GetWorldspaceFlag);
	REG_CMD(SetWorldspaceFlag);
	REG_CMD(GetPCCanFastTravel);
	REG_CMD(GetRadiationLevelAlt);
	REG_CMD(GetButcherPeteVersion);
	REG_CMD(MessageExAlt);
	REG_CMD(MessageBoxEx);
	REG_CMD(IsKeyPressedAlt);
	REG_CMD(GetKiller);
	REG_CMD(SetTexturePath);
	REG_CMD(GetCrosshairRefEx);
	REG_CMD(GetLockedAlt);
	REG_CMD(IsLoadDoor);
	REG_CMD(GetLightingTemplateTraitNumeric);
	REG_CMD(SetLightingTemplateTraitNumeric);
	REG_CMD(SetBipedIconPathAlt);
	REG_CMD(SetCustomMapMarkerIcon);
	REG_CMD(PatchFreezeTime);
	REG_CMD(SetHotkeyItem);
	REG_CMD(ClearHotkey);
	REG_CMD(ClearMessageQueue);
	REG_CMD(ResetFallTime);
	REG_CMD(GetKillXP);
	REG_CMD(GetIsRagdolled);
	REG_CMD(GetDestructionDataHealth);
	REG_CMD(SetDestructionDataHealth);
	REG_CMD(GetDestructionDataTargetable);
	REG_CMD(SetDestructionDataTargetable);
	REG_CMD(GetDestructionDataNumStages);
	REG_CMD(GetNthDestructionStageTrait);
	REG_CMD(SetNthDestructionStageTrait);
	REG_CMD(GetNthDestructionStageExplosion);
	REG_CMD(SetNthDestructionStageExplosion);
	REG_CMD(GetNthDestructionStageDebris);
	REG_CMD(SetNthDestructionStageDebris);
	REG_CMD(GetActorVelocity);
	REG_CMD(RefreshIdle);
	REG_CMD(IsDLLLoaded);
	REG_CMD(StopSound);
	REG_CMD(StopSoundAlt);
	REG_CMD(IsSoundPlaying);
	REG_CMD(GetCollisionObjProperty);
	REG_CMD(GetPipBoyMode);
	REG_CMD(GetSystemColor);
	REG_CMD(SetSystemColor);
	REG_CMD(GetLightFlag);
	REG_CMD(SetLightFlag);
	REG_CMD(GetLightTraitNumeric);
	REG_CMD(SetLightTraitNumeric);
	REG_CMD(SetActorVelocity);
	REG_CMD(GetFallTimeElapsed);
	REG_CMD(GetFallTimeRemaining);
	REG_CMD(GetActorGravityMult);
	REG_CMD(SetActorGravityMult);
	REG_CMD(IsInWater);
	REG_CMD(GetCreatureType);
	REG_CMD(PlayIdleEx);
	REG_CMD(GetPlayedIdle);
	REG_CMD(IsIdlePlayingEx);
	REG_CMD(SetUIFloatGradual);
	REG_CMD(AddTileFromTemplate);
	REG_CMD(MoveToCell);
	REG_CMD(GetGameVolume);
	REG_CMD(SetGameVolume);
	REG_CMD(SetClimateSunGlareTexture);
	REG_CMD(SetClimateSunTexture);
	REG_CMD(GetClimateTraitNumeric);
	REG_CMD(SetClimateTraitNumeric);
	REG_CMD(RefreshCurrentClimate);
	REG_CMD(GetCurrentClimate);
	REG_CMD(SetCurrentClimate);
	REG_CMD(GetCellImageSpace);

	if (fose->isEditor) {
		WriteEditorPatches();
	}
	else {
		WritePatches();
	}

	cmdTableInterface = (FOSECommandTableInterface*)fose->QueryInterface(kInterface_CommandTable);
	if (cmdTableInterface) {


		// fix memory corruption in fose functions
		CommandInfo* info = cmdTableInterface->GetByOpcode(0x1428);
		info->execute = Hook_ListAddForm_Execute;

		info = cmdTableInterface->GetByOpcode(0x142A);
		info->execute = Hook_ListAddReference_Execute;

		// additional null check
		info = cmdTableInterface->GetByOpcode(0x1405);
		info->execute = Hook_GetRepairList_Execute;

		// fix GetRadiationLevel not working on NPCs
		info = cmdTableInterface->GetByOpcode(0x11F7);
		info->eval = Hook_GetRadiationLevel_Eval;
		info->execute = Hook_GetRadiationLevel_Execute;

		info = cmdTableInterface->GetByOpcode(0x116A);
		info->eval = Cmd_GetButcherPeteVersion_Eval;
		info->execute = Cmd_GetButcherPeteVersion_Execute;

		info = cmdTableInterface->GetByOpcode(0x14AC);
		info->execute = Hook_GetHotkeyItem_Execute;

		// more null checks
		info = cmdTableInterface->GetByOpcode(0x14AD);
		info->execute = Hook_GetTeleportCell_Execute;

		cmd_IsKeyPressed = cmdTableInterface->GetByOpcode(0x143A);
	}
	return true;
}

};
