#pragma once
#include "CommandTable.h"
#include "ParamInfos.h"

DEFINE_COMMAND_PLUGIN(SetNifBlockRotation, 1, kParams_OneString_ThreeFloats_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(SetNifBlockTranslation, 1, kParams_OneString_ThreeFloats_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetNifBlockScale, 1, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetNifBlockScale, 1, kParams_OneString_OneFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetNifBlockFlag, 1, kParams_OneString_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetNifBlockFlag, 1, kParams_OneString_TwoInts_OneOptionalInt);
