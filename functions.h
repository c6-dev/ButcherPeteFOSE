#pragma once
#undef MessageBoxEx
#include "CommandTable.h"
#include "ParamInfos.h"

DEFINE_COMMAND_PLUGIN(IsOwned, 1, kParams_OneActorRef);
DEFINE_COMMAND_PLUGIN(AddItemOwnership, 1, kParams_OneForm_OneFloat_OneForm_OneOptionalRank);
DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, 0, kParams_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, 0, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetPCCanFastTravel, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadiationLevelAlt, 1, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetButcherPeteVersion, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(MessageExAlt, 0, kParams_OneFloat_OneFormatString);
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
DEFINE_COMMAND_PLUGIN(GetCellImageSpace, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetActiveMenuMode, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetDestructionDataHealth, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetDestructionDataHealth, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataTargetable, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetDestructionDataTargetable, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataNumStages, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageTrait, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageTrait, 0, kParams_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageExplosion, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageExplosion, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageDebris, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageDebris, 0, kParams_OneForm_OneInt_OneOptionalForm);

bool Hook_ListAddForm_Execute(COMMAND_ARGS);

bool Hook_ListAddReference_Execute(COMMAND_ARGS);

bool Hook_GetHotkeyItem_Execute(COMMAND_ARGS);

bool Hook_GetRadiationLevel_Execute(COMMAND_ARGS);

bool Hook_GetRadiationLevel_Eval(COMMAND_ARGS_EVAL);

bool Hook_GetRepairList_Execute(COMMAND_ARGS);

bool Hook_GetTeleportCell_Execute(COMMAND_ARGS);