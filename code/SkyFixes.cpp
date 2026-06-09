#include "SkyFixes.hpp"
#include "GameObjects.h"
#include "SafeWrite.h"

namespace SkyFixes
{
	void __fastcall UpdateSkySoundHook(Sky* sky, void* edx, TESWeather* weather)
	{
		if ((sky->uiFlags & (Sky::REFRESH_WEATHER | Sky::FORCED_UPDATE)) != 0 || BGSSaveLoadGame::GetSingleton()->IsLoading())
		{
			ThisCall<void>(0x57C080, sky, weather);
		}
	}

	void SkyFinishLoadGame()
	{
		Sky* pSky = Sky::Get();
		if (pSky->pClouds && (pSky->eMode == Sky::Mode::SM_FULL || pSky->eMode == Sky::Mode::SM_SKYDOME_ONLY))
		{
			pSky->pClouds->bForceTransTextureUpdate = true;
			pSky->pClouds->Update(pSky, 0.f);
		}

		ThisCall<void>(0x57E0A0, pSky, 0.f);
		ThisCall<void>(0x57D5C0, pSky);
	}

	void __fastcall ResetWeatherHook(Sky* sky)
	{
		ThisCall<void>(0x57DD10, sky);
		for (auto iter = sky->pSkySoundList->Head(); iter; iter = iter->Next())
		{
			SkySound* pSound = iter->Data();
			if (pSound)
			{
				if (pSound->sound.IsPlaying()) pSound->sound.Stop();

				pSound->sound.Release();

				GameHeapFree(pSound);
			}
			iter->RemoveMe();
		}
		*(UInt8*)0x1072F8C = 0;
	}

	void RevertSky()
	{
		ResetWeatherHook(Sky::Get());
	}

	void __declspec(naked) RevertSkyHook()
	{
		__asm {
			call RevertSky
			mov al, 1
			retn
			}
	}

	void __declspec(naked) FinishLoadGameHook()
	{
		__asm {
			call SkyFinishLoadGame
			mov al, 1
			retn
			}
	}

	void __fastcall SkySetModeHook(Sky* sky)
	{
		if (sky->eMode == Sky::Mode::SM_FULL || sky->eMode == Sky::Mode::SM_SKYDOME_ONLY)
		{
			ThisCall<void>(0x57C080, sky, sky->pCurrentWeather);
			ThisCall<void>(0x57C080, sky, sky->pLastWeather);
		}
		ThisCall<void>(0x57D5C0, sky);
	}

	void Init()
	{
		// Add GetSaveGameLoading check for ActivateWeatherSounds in Sky::UpdateSound
		PatchMemoryNop(0x57CC61, 9);
		WriteRelCall(0x57CC70, (UInt32)UpdateSkySoundHook);

		// Implement Sky::FinishLoadGame, handle updates here
		SafeWrite32(0x6D13B0, UInt32(FinishLoadGameHook));

		// Skip Update calls in Sky::LoadGame, handled by FinishLoadGame
		WriteRelJump(0x57E641, 0x57E655);

		// Implement Sky::Revert, resets the weather data
		SafeWrite32(0x6D235C, uint32_t(RevertSkyHook));

		// Activate weather sounds in Sky::SetMode
		WriteRelCall(0x57DD01, (UInt32)SkySetModeHook);

		// Skip Sky::ActivateWeatherSounds in TES::LeaveInterior
		// Handled by our SetMode hook
		//SafeWrite8(0x43A9A1, 0xEB);

		// clear sky sounds in ResetWeather
		WriteRelCall(0x57E458, (UInt32)ResetWeatherHook);
		WriteRelCall(0x7823AD, (UInt32)ResetWeatherHook);
	}
}
