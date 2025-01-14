#include "patches.h"
#include <unordered_map>
#include "havok.h"
#include "SafeWrite.h"
#include "GameSettings.h"

std::unordered_map<UInt32, char*> markerIconMap;
char** defaultMarkerList = (char**)0xF6B13C;

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

}

void WriteEditorPatches()
{
	SafeWriteBuf(0xD4A838, "GetButcherPeteVersion\0", 22);
}