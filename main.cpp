#include "PluginAPI.h"
#include "CommandTable.h"
#include "GameAPI.h"
#include "ParamInfos.h"
#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"
#include <string>
#include "SafeWrite.h"
#include "main.h"
#include "ParamInfos.h"

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
	if(fose->foseVersion < FOSE_VERSION_INTEGER)
	{
		_ERROR("FOSE version too old (got %08X expected at least %08X)", fose->foseVersion, FOSE_VERSION_INTEGER);
		return false;
	}

	if(!fose->isEditor && fose->runtimeVersion != FALLOUT_VERSION_1_7)
	{
		_ERROR("incorrect runtime version (got %08X need %08X)", fose->runtimeVersion, FALLOUT_VERSION_1_7);
		return false;
	}

	
	return true;
}
const char* fnName = "GetButcherPeteVersion";

bool FOSEPlugin_Load(const FOSEInterface * fose)
{
	fose->SetOpcodeBase(0x2200);
	g_pluginHandle = fose->GetPluginHandle();
	REG_CMD(IsOwned);
	REG_CMD(AddItemOwnership);
	REG_CMD(GetWorldspaceFlag);
	REG_CMD(SetWorldspaceFlag);
	REG_CMD(GetPCCanFastTravel);
	REG_CMD(GetRadiationLevelAlt);
	REG_CMD(GetButcherPeteVersion);
	if (fose->isEditor) {
		WriteEditorPatches();
	}
	else {
		WritePatches();
	}
	FOSECommandTableInterface* cmdTableInterface = (FOSECommandTableInterface*)fose->QueryInterface(kInterface_CommandTable);
	if (cmdTableInterface) {


		// fix memory corruption in fose functions
		CommandInfo* info = cmdTableInterface->GetByOpcode(0x1428);
		info->execute = Hook_ListAddForm_Execute;

		info = cmdTableInterface->GetByOpcode(0x142A);
		info->execute = Hook_ListAddReference_Execute;

		// fix GetRadiationLevel not working on NPCs
		info = cmdTableInterface->GetByOpcode(0x11F7);
		info->eval = Hook_GetRadiationLevel_Eval;
		info->execute = Hook_GetRadiationLevel_Execute;

		info = cmdTableInterface->GetByOpcode(0x116A);
		info->eval = Cmd_GetButcherPeteVersion_Eval;
		info->execute = Cmd_GetButcherPeteVersion_Execute;

	}
	return true;
}

};
