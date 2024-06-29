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
	info->version = 1;
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

	if(!fose->isEditor && fose->runtimeVersion != FALLOUT_VERSION)
	{
		_ERROR("incorrect runtime version (got %08X need %08X)", fose->runtimeVersion, FALLOUT_VERSION);
		return false;
	}

	
	return true;
}


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
	if (!fose->isEditor) WritePatches();
	return true;
}

};
