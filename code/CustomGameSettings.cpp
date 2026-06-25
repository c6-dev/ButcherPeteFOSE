
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

	void Init()
	{
		fDamageGunWeapCondBuffer.Initialize("fDamageGunWeapCondBuffer", 1.0f);
		fDamageMeleeWeapCondBuffer.Initialize("fDamageMeleeWeapCondBuffer", 1.0f);
		fArmorRatingCondBuffer.Initialize("fArmorRatingCondBuffer", 1.0f);
		fPlayerGunshotVolumeMult.Initialize("fPlayerGunshotVolumeMult", 1.0f);
		fNPCGunshotVolumeMult.Initialize("fNPCGunshotVolumeMult", 1.0f);
		fProjectileImpactVolumeMult.Initialize("fProjectileImpactVolumeMult", 1.0f);
	}
}
