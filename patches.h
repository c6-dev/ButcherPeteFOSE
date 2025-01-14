#pragma once
#include "netimmerse.h"
#include "GameForms.h"

const char* GetPackageTypeName(char type);
TESPackage* __fastcall GetAIPackageHook(Actor* actor);
char* __fastcall GetMapMarker(TESObjectREFR* thisObj, UInt16 mapMarkerType);
void GetMapMarkerHook();
void __fastcall SetTreeFullLODToINISetting(TESObjectCELL* cell);
void MarkNode(NiNode* node);
void __stdcall MarkPlayerBones();
void __fastcall SetCellImageSpaceHook(TESObjectCELL* cell, void* edx, TESImageSpace* imageSpace);
void uGridsLoadingCrashHook();
void WritePatches();
void WriteEditorPatches();