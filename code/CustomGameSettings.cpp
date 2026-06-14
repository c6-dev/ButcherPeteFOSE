
#include "CustomGameSettings.hpp"

#include "SafeWrite.h"

namespace CustomGameSettings
{
	CustomGameSetting fDamageGunWeapCondBuffer;
	CustomGameSetting fDamageMeleeWeapCondBuffer;
	CustomGameSetting fArmorRatingCondBuffer;
	CustomGameSetting fPlayerGunshotVolumeMult;
	CustomGameSetting fNPCGunshotVolumeMult;
	CustomGameSetting fProjectileImpactVolumeMult;

	void InitSettings()
	{
		fDamageGunWeapCondBuffer.Initialize("fDamageGunWeapCondBuffer", 1.0f);
		fDamageMeleeWeapCondBuffer.Initialize("fDamageMeleeWeapCondBuffer", 1.0f);
		fArmorRatingCondBuffer.Initialize("fArmorRatingCondBuffer", 1.0f);
		fPlayerGunshotVolumeMult.Initialize("fPlayerGunshotVolumeMult", 1.0f);
		fNPCGunshotVolumeMult.Initialize("fNPCGunshotVolumeMult", 1.0f);
		fProjectileImpactVolumeMult.Initialize("fProjectileImpactVolumeMult", 1.0f);
	}

	void Init()
	{
		WriteRelCall(0xCABF68, (UInt32)InitSettings);
		// hooking the collection init in a random gmst - effectively a nullsub after at least one gmst has been initialized
	}
}
