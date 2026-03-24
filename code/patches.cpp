#include <unordered_map>
#include "custom_terminal_models.h"
#include "GameMenus.h"
#include "SafeWrite.h"
#include "GameSettings.h"
#include "patches.h"
#include <unordered_set>

std::unordered_map<UInt32, char*> markerIconMap;

char** defaultMarkerList = (char**)0xF6B13C;

TESClimate* s_forcedClimate = nullptr;

bool bCombatMusicDisabled = false;

std::unordered_set<const char*> s_overrideBSAFiles;


void __fastcall SetClimateHook(Sky* sky, void* edx, TESClimate* climate, bool a3)
{
	if (s_forcedClimate) {
		climate = s_forcedClimate;
	}
	ThisCall<void>(0x57E3B0, sky, climate, a3);
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
	default:
		return "";
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


char* __fastcall GetMapMarker(TESObjectREFR* thisObj, UInt16 mapMarkerType) {
	auto it = markerIconMap.find(thisObj->refID);
	if (it != markerIconMap.end()) return it->second;
	return defaultMarkerList[mapMarkerType];
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
void __fastcall SetCellImageSpaceHook(TESObjectCELL* cell, void* edx, TESImageSpace* imageSpace) {
	ThisCall<void>(0x4D36C0, cell, imageSpace);
	PlayerCharacter* player = PlayerCharacter::GetSingleton();
	if (player->parentCell != nullptr && player->parentCell == cell) {
		CdeclCall<void>(0xAD5900, imageSpace->fTraitValues);
	}

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

void __fastcall MarkRefAsModifiedHook(TESObjectREFR* refr, int edx, UInt32 flag)
{
	if (flag != 4 || refr->parentCell == nullptr || (refr->parentCell->flags & 0x400000) == 0) {
		ThisCall(0x4542B0, refr, flag);
	}
}

char __fastcall QueueUIMessageHook(void* menu, void* edx, char* text, int type, char* path, char* soundName, float time)
{
	if (fabs(time) < FLT_EPSILON)
	{
		return 0;
	}
	return ThisCall<char>(0x648500, menu, text, type, path, soundName, time);
}

bool __fastcall CombatMusicHook(uint32_t* a1) {
	if (bCombatMusicDisabled) return false;
	return ThisCall<bool>(0x7A8580, a1);
}

// Fixes conditions that got inverted when refactoring the function from Oblivion->FO3
// The biggest issue was: when trespassing, player can open any door in existence, due to CanActorIgnoreDoorLock being ran last, if at all - IsTrespassing gets called before it, returning true.
// The fix simply restores the original Oblivion order.
bool TESObjectDOOR__CanActorIgnoreLock(void* apLock, Actor* apActor, TESObjectREFR* apDoorRef, bool abActivate, bool abMovement)
{
	if (!apActor || !apLock)
		return false;

	if (ThisCall<bool>(0x4AA0F0, apLock, apActor, abActivate, abMovement)) // DoorLock::CanActorIgnoreDoorLock
		return true;

	if (apActor == PlayerCharacter::GetSingleton())
		return false;
	// Actor::IsFollowing - TESObjectREFR::GetTeleport
	if (ThisCall<bool>(0x6F57B0, apActor) && apActor->baseProcess->GetTarget() == PlayerCharacter::GetSingleton() && ThisCall<UInt32*>(0x4E74A0, apDoorRef))
		return true;

	if (apActor->IsTrespassing())
		return true;

	return false;
}

// Fixes crash in GetOffersServices when an actor doesn't have a base process
bool __fastcall GetOffersServicesNow(Actor* apThis)
{
	if (apThis->baseProcess)
		return ThisCall<bool>(0x6F7980, apThis);

	return false;
}

void __cdecl OnFreeRefRemoveFromSelectableList(TESObjectREFR* ref)
{
	auto hud = HUDMainMenu::GetSingleton();
	if (hud && hud->crosshairRef == ref)
	{
		hud->crosshairRef = nullptr;
	}
	InterfaceManager::GetSingleton()->selectableRefs.Remove(ref);
}

double __fastcall Creature__GetTotalArmorDR(Creature* apThis) {
	if (apThis->fTotalArmorDR >= 0.0)
		return apThis->fTotalArmorDR;

	UInt32 eIndex = 18; // Damage Resistance
	float fInternalValue = 0.f;
	bool bFound = false;
	float v6 = apThis->InternalGetActorValue(eIndex, bFound);
	if (bFound)
		fInternalValue = v6;

	apThis->fTotalArmorDR = apThis->avOwner.GetActorValueDamage(eIndex) + apThis->avOwner.GetPermActorValue(eIndex) + fInternalValue;

	return apThis->fTotalArmorDR;
}

void __fastcall OnResetLoadedRefData(TESObjectREFR* ref, void* edx, NiNode* node)
{
	ThisCall(0x4F8E00, ref, node);

	if (ref->CanHaveSound())
	{
		ref->AttachSound(node != nullptr);
	}
}

// Fixes DR not updating when armor is changed by non-player actors
	// Works both for Creatures and Characters (same offsets)
	// Thanks to IntoTheRough for identifying the issue
void __fastcall ResetArmorRating(Character* apCharacter)
{
	apCharacter->fTotalArmorDR = -1.f;
	BaseProcess* pProcess = apCharacter->baseProcess;
	if (pProcess)
	{
		pProcess->SetCachedActorValueOutOfDate(eActorVal_DamageResistance);
	}
}

TESObjectREFR* GetHackingMenuRef(HackingMenu* pHackingMenu)
{
	if (pHackingMenu->tileChildList174[2])
	{
		return pHackingMenu->pTargetRef;
	}
	InterfaceManager* pManager = InterfaceManager::GetSingleton();
	return pManager->crosshairRef;
}

void __fastcall SetTerminalModelHook(void* a1)
{
	char* newPath = (char*)g_terminalModelDefault;
	if (!s_terminalAltModelsMap.empty()) {
		TESObjectREFR* targetRef = nullptr;
		
		
		ComputersMenu* pComputersMenu = ComputersMenu::GetInstance();
		if (pComputersMenu)                 
		{
			targetRef = pComputersMenu->pTargetRef;
		}
		else
		{
			HackingMenu* pHackingMenu = HackingMenu::GetInstance(); 
			if (pHackingMenu) {
				targetRef = GetHackingMenuRef(pHackingMenu);
			}
		}
		if (targetRef)
		{
			auto it = s_terminalAltModelsMap.find(reinterpret_cast<BGSTerminal*>(targetRef->baseForm));
			if (it != s_terminalAltModelsMap.end()) {
				ThisCall(0x69D7D0, nullptr); // PurgeTerminalModel
				newPath = it->second;
			}
			
		}
		
	}
	*(char**)0xF6B70C = newPath;  // pTerminalFile
	ThisCall(0x69D980, a1);
}

bool __fastcall GetINISettingHook(IniSettingCollection* ini, void* edx, char* name, Setting** setting) {
	if (ini && !ini->settings.Empty()) {
		auto iter = ini->settings.Head();
		while (iter && iter->data != nullptr) {
			Setting* pSetting = iter->data;
			iter = iter->Next();
			if (_stricmp(pSetting->name, name) == 0) {
				*setting = pSetting;
				return true; 
			}
		}
	}
	RendererSettingCollection* rendererSettings = RendererSettingCollection::GetCollection();
	if (rendererSettings && !rendererSettings->settings.Empty()) {
		auto iter = rendererSettings->settings.Head();
		while (iter && iter->data != nullptr) {
			Setting* pSetting = iter->data;
			iter = iter->Next();
			if (_stricmp(pSetting->name, name) == 0) {  
				*setting = pSetting;
				return true;
			}
		}
	}
	return false;
}
void __stdcall HandleSettingType(Setting* setting, Setting::EType type) {
	switch (type) {
	case Setting::kSetting_Bool:
		if (IsConsoleMode()) Console_Print("INISetting %s >> %i", setting->name, setting->data.b);
		break;
	case Setting::kSetting_Integer:
		if (IsConsoleMode()) Console_Print("INISetting %s >> %d", setting->name, setting->data.i);
		break;
	case Setting::kSetting_Unsigned:
		if (IsConsoleMode()) Console_Print("INISetting %s >> %X", setting->name, setting->data.u);
		break;
	case Setting::kSetting_Float:
		if (IsConsoleMode()) Console_Print("INISetting %s >> %.2f", setting->name, setting->data.f);
		break;
	case Setting::kSetting_String:
		if (IsConsoleMode()) Console_Print("INISetting %s >> '%s'", setting->name, setting->data.str);
		break;
	case Setting::kSetting_r:
		if (IsConsoleMode()) Console_Print("INISetting %s >> R: %d G: %d B: %d", setting->name, setting->data.rgb[3], setting->data.rgb[2], setting->data.rgb[1]);
		break;
	case Setting::kSetting_a:
		if (IsConsoleMode()) Console_Print("INISetting %s >> R: %d G: %d B: %d alpha: %d", setting->name, setting->data.rgb[3], setting->data.rgb[2], setting->data.rgb[1], setting->data.rgb[0]);
		break;
	default:
		if (IsConsoleMode()) Console_Print("INISetting %s >> UNKNOWN TYPE", setting->name);
		break;
	}
}

__declspec(naked) void GetINISettingTypeHook() {
	__asm {
		push eax
		mov eax, esi
		push eax
		call HandleSettingType
		mov eax, 0x53E83C
		jmp eax
	}
}

bool __stdcall SaveINIHook() {
	
	RendererSettingCollection* rendererSettings = RendererSettingCollection::GetCollection();
	ThisCall<bool>(0x539FA0, rendererSettings, rendererSettings->iniPath);
	return StdCall<bool>(0x53E440);
}

void __fastcall DOFFOVHook(void* a1, void* edx, float a3, bool a4, NiCamera* a5, bool a6)
{
	DWORD g_VATSCameraMode = *(DWORD*)0x108D0A0;
	if (!g_VATSCameraMode) return;
	ThisCall(0xAAEAF0, a1, a3, a4, a5, a6);
}

__declspec(naked) void FixNewGameMusic() {
	__asm {
		test esi, esi
		jz SETDEFAULT
		cmp dword ptr ds:[0x1079054], esi
		jnz NEXTCHECK
		mov eax, 0x6BDCDD
		jmp eax
		SETDEFAULT:
		mov eax, 0x6BDD01
		jmp eax
		NEXTCHECK:
		mov eax, 0x6BDCE8
		jmp eax
	}

}



void* __cdecl LoadBSAFileHook(const char* filename, short arg2, bool isOverride)
{
	if (s_overrideBSAFiles.find(filename) != s_overrideBSAFiles.end())
	{
		isOverride = true;
	}
	return CdeclCall<void*>(0xBCBF10, filename, arg2, isOverride);
}

void LoadBSAOverrides() {
	char dataPath[0x80];
	memcpy(dataPath, "data\\", 6);
	for (DirectoryIterator dirIter("Data\\*.override"); dirIter; ++dirIter)
		if (dirIter.IsFile())
		{
			memcpy(strcpy(dataPath + 5, *dirIter) - 8, "bsa", 4);
			s_overrideBSAFiles.insert(dataPath);
		}
	if (!s_overrideBSAFiles.empty())
		WriteRelCall(0x44672C, (UInt32)LoadBSAFileHook);
}
void WritePatches() {

	WriteRelJump(0x437736, UInt32(uGridsLoadingCrashHook)); // fix crash when loading a save with increased ugrids after lowering them
	WriteRelJump(0x4FDD9F, 0x4FDDB9); // increase grass render distance
	WriteRelCall(0x5110A5, (UInt32)GetAIPackageHook);

	// SetCustomMapMarkerIcon
	SafeWrite16(0x6654DB, 0x9090); // nop 2b
	WriteRelCall(0x6654DD, (UInt32)GetMapMarkerHook); // call 5b
	SafeWrite8(0x6654E2, 0x50); // push eax instead of edx

	WriteRelCall(0x4DBE16, (UInt32)SetTreeFullLODToINISetting); // fixed bForceFullLOD resetting when opening pipboy (thanks Stewie)
	WriteRelCall(0x76FA6A, (UInt32)MarkPlayerBones);
	WriteRelCall(0x51F0B0, (UInt32)SetCellImageSpaceHook);
	WriteRelCall(0x440ED6, (UInt32)SetClimateHook);
	WriteRelCall(0x530FE0, (UInt32)SetClimateHook);
	WriteRelCall(0x7878CB, (UInt32)SetClimateHook);
	SafeWrite32(0xDCEEC4, (UInt32)MarkRefAsModifiedHook); // make cells tagged with Unknown 23 not save ref positions

	WriteRelCall(0x61B8D2, (UInt32)QueueUIMessageHook); // don't show messages with duration of 0

	// ToggleCombatMusic
	WriteRelCall(0x6C047D, (UInt32)CombatMusicHook);
	WriteRelCall(0x6C08DF, (UInt32)CombatMusicHook);

	WriteRelJump(0x4B7C70, UInt32(TESObjectDOOR__CanActorIgnoreLock));

	WriteRelCall(0x512599, UInt32(GetOffersServicesNow));

	WriteRelCall(0x61E33D, UInt32(OnFreeRefRemoveFromSelectableList));

	// prevent Actor::RestoreHealth being called in PlayerCharacter::PlayerSleep since it also gets called in Actor::UpdateMagic
	// which lead to healing twice as much when waiting
	PatchMemoryNop(0x76C9E4, 19);

	// Fix for doubled DR on creatures
	SafeWrite32(0xE14EAC, UInt32(Creature__GetTotalArmorDR));
	 
	// ensures refs play their attached sounds when nodes are set
	WriteRelCall(0x4FACB9, UInt32(OnResetLoadedRefData));
	WriteRelCall(0x4FACD2, UInt32(OnResetLoadedRefData));

	// Fixes DR not updating when armor is changed by non - player actors
	WriteRelJump(0x728240, UInt32(ResetArmorRating));

	SafeWrite32(0xE03590, UInt32(SetTerminalModelHook));

	g_terminalModelDefault = *(const char**)0xF6B70C;

	// fix for Get/Set/SaveINISetting not reading renderer INI setting list
	WriteRelCall(0x53E786, (uint32_t)GetINISettingHook);
	WriteRelCall(0x53E92E, (uint32_t)GetINISettingHook);
	SafeWrite32(0xF53CB8, (uint32_t)SaveINIHook);
	WriteRelJump(0x53E7A3, (uint32_t)GetINISettingTypeHook);

	// fix DOF changing world/1stPerson cam FOV outside of VATS, resulting in turn speed looking different
	WriteRelCall(0x6EADD9, (UInt32)DOFFOVHook);
	WriteRelCall(0x6EAE4E, (UInt32)DOFFOVHook);

	WriteRelJump(0x6BDCD5, UInt32(FixNewGameMusic));
	
}

void WriteEditorPatches()
{
	SafeWriteBuf(0xD4A838, "GetButcherPeteVersion\0", 22);
}