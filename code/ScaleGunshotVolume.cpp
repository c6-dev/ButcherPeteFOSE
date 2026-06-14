#include "ScaleGunshotVolume.hpp"
#include "CustomGameSettings.hpp"
#include "GameSound.h"
#include "SafeWrite.h"

namespace ScaleGunshotVolume
{
	void __fastcall ScaleGunshotVolume(BSSoundHandle* sound, void* edx, bool isPlayer)
	{
		if (isPlayer)
		{
			ThisCall<bool>(0xBD01C0, sound, CustomGameSettings::fPlayerGunshotVolumeMult.Float());
		}
		else 
		{
			ThisCall<bool>(0xBD01C0, sound, CustomGameSettings::fNPCGunshotVolumeMult.Float());
		}
	}

	void __declspec(naked) SetGunshotVolumeHook()
	{
		__asm {
			movzx eax, byte ptr[esp+0x4C]
			push eax
			lea ecx, dword ptr[esp+0x28]
			call ScaleGunshotVolume
			mov eax, 0x6C644C
			jmp eax
			}
	}

	bool __fastcall ScaleCollisionVolume(BSSoundHandle* sound, void* edx, bool a2)
	{
		sound->SetVolume(CustomGameSettings::fProjectileImpactVolumeMult.Float());
		return sound->Play();
	}
	void Init()
	{
		WriteRelJump(0x6C643D, (UInt32)SetGunshotVolumeHook);
		//	WriteRelCall(0x6C5249, (UInt32)ScaleCollisionVolume);
		//	WriteRelCall(0x6C5281, (UInt32)ScaleCollisionVolume);
		WriteRelCall(0x7CD6BA, (UInt32)ScaleCollisionVolume);
		WriteRelCall(0x7CD70E, (UInt32)ScaleCollisionVolume);
	}
}
