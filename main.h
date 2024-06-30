#pragma once
#define REG_CMD(name) fose->RegisterCommand(&kCommandInfo_##name);

DEFINE_COMMAND_PLUGIN(IsOwned, , 1, 1, kParams_OneActorRef);
DEFINE_COMMAND_PLUGIN(AddItemOwnership, , 1, 4, kParams_OneForm_OneFloat_OneForm_OneOptionalRank);
DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, , 0, 2, kParams_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, , 0, 3, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetPCCanFastTravel, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadiationLevelAlt, , 1, 0, NULL);

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
	SafeWrite16((UInt32)0x61B850, 0x6AFF);	// PUSH 0FFFFFFFFh
	if (IsConsoleMode()) {
		Console_Print("GetPCCanFastTravel >> %.f", *result);
	}
	return true;
}

float GetRadiationLevel(Actor* actor)
{
	if (actor->IsActor()) {
		if (HighProcess* hiProc = (HighProcess*)actor->baseProcess; hiProc && !hiProc->uiProcessLevel) {
			if (actor == PlayerCharacter::GetSingleton()) {
				return hiProc->fWaterRadsSec + hiProc->rads238 + hiProc->GetRadsSec();
			}
			else {
				return actor->GetRadiationLevel();
			}
		}
	}
	return true;
}

bool Cmd_GetRadiationLevelAlt_Execute(COMMAND_ARGS) {
	*result = GetRadiationLevel((Actor*)thisObj);
	if (IsConsoleMode()) {
		Console_Print("GetRadiationLevelAlt >> %.2f", *result);
	}
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

void WritePatches() {
	WriteRelJump(0x437736, UInt32(uGridsLoadingCrashHook));
}