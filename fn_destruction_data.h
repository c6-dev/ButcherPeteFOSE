#pragma once
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