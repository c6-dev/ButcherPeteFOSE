#include "ItemConditionBuffer.hpp"
#include "CustomGameSettings.hpp"
#include "GameForms.h"
#include "GameSettings.h"
#include "SafeWrite.h"

namespace ItemConditionBuffer
{
	float __cdecl ScaleExplosionDamageHook(BGSExplosion* a1, float health)
	{
		auto gs_fDamageGunWeapCondMult = (Setting*)0xF5FBDC;
		auto gs_fDamageGunWeapCondBase = (Setting*)0xF5FBD0;
		float mult = gs_fDamageGunWeapCondMult->data.f;
		float base = gs_fDamageGunWeapCondBase->data.f;
		float fDamage = a1 == nullptr ? 0.f : a1->fDamage;
		float buffer = CustomGameSettings::fDamageGunWeapCondBuffer.Float();

		float result = std::min<float>(health * (1.0f / buffer), 1.0f) * mult + base;
		return result * fDamage;
	}

	float __cdecl ScaleWeapCond(float health)
	{
		auto gs_fDamageGunWeapCondMult = (Setting*)0xF5FBDC;
		auto gs_fDamageGunWeapCondBase = (Setting*)0xF5FBD0;
		float mult = gs_fDamageGunWeapCondMult->data.f;
		float base = gs_fDamageGunWeapCondBase->data.f;
		float buffer = CustomGameSettings::fDamageGunWeapCondBuffer.Float();

		return std::min<float>(health * (1.0f / buffer), 1.0f) * mult + base;
	}

	double __cdecl CalcArmorRatingHook(unsigned __int16 armorRating, float health)
	{
		auto gs_fArmorRatingMax = (Setting*)0xF60DD0;
		auto gs_fArmorRatingBase = (Setting*)0xF60DDC;

		float max = gs_fArmorRatingMax->data.f;
		float base = gs_fArmorRatingBase->data.f;
		float mult = max - base;

		float buffer = CustomGameSettings::fArmorRatingCondBuffer.Float();

		float scaledHealth = std::min<float>(health * (1.0f / buffer), 1.0f);

		return (scaledHealth * mult + base) * armorRating;
	}

	__declspec(naked) void ScaleWeapCondHook()
	{
		__asm {
			sub esp, 4
			fstp dword ptr[esp]
			call ScaleWeapCond
			add esp, 4
			fstp dword ptr[esp + 0x24]
			mov eax, 0x583B67
			jmp eax
			}
	}

	__declspec(naked) void ScaleWeapCondHook2()
	{
		__asm {
			movzx edx, byte ptr[esi + 0xB0]
			mov dword ptr[esp + 0x14], edx
			fld dword ptr[esp + 0x38]
			sub esp, 4
			fstp dword ptr[esp]
			call ScaleWeapCond
			add esp, 4
			mov eax, 0x583E12
			jmp eax
			}
	}


	void Init()
	{
		WriteRelJump(0x5838A0, (UInt32)ScaleExplosionDamageHook);
		WriteRelJump(0x583B3A, (UInt32)ScaleWeapCondHook);
		WriteRelJump(0x583DF7, (UInt32)ScaleWeapCondHook2);
		WriteRelJump(0x582720, (UInt32)CalcArmorRatingHook);
	}
}
