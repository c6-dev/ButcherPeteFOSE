#pragma once

#define REG_CMD(name) fose->RegisterCommand(&kCommandInfo_##name);
#undef MessageBoxEx

DEFINE_COMMAND_PLUGIN(IsOwned, , 1, 1, kParams_OneActorRef);
DEFINE_COMMAND_PLUGIN(AddItemOwnership, , 1, 4, kParams_OneForm_OneFloat_OneForm_OneOptionalRank);
DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, , 0, 2, kParams_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, , 0, 3, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetPCCanFastTravel, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadiationLevelAlt, , 1, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetButcherPeteVersion, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(MessageExAlt, , 0, 22, kParams_OneFloat_OneFormatString);
DEFINE_COMMAND_PLUGIN(MessageBoxEx, , 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(IsKeyPressedAlt, , 0, 1, kParams_OneInt);

int g_version = 110;

char* s_strArgBuffer;
char* s_strValBuffer;
const UInt32 kMsgIconsPathAddr[] = { 0xDC0C38, 0xDC0C78, 0xDC5544, 0xDCE658, 0xDD9148, 0xDE3790, 0xDF3278 };

FOSECommandTableInterface* cmdTableInterface = nullptr;
CommandInfo* cmd_IsKeyPressed = nullptr;

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
			cmd_IsKeyPressed->eval(thisObj, &keyCode, nullptr, result);
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
	SafeWrite16((UInt32)0x61B850, 0x6AFF);	// PUSH 0FFFFFFFFh
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
void WritePatches() {
	s_strArgBuffer = (char*)malloc(0x4000);
	s_strValBuffer = (char*)malloc(0x10000);

	WriteRelJump(0x437736, UInt32(uGridsLoadingCrashHook)); // fix crash when loading a save with increased ugrids after lowering them
	WriteRelJump(0x4FDD9F, 0x4FDDB9); // increase grass render distance
}

void WriteEditorPatches()
{
	SafeWriteBuf(0xD4A838, "GetButcherPeteVersion\0", 22);
}