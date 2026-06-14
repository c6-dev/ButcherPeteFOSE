#pragma once
#include "Utilities.h"

class CustomGameSetting
{
public:
	union Info
	{
		const char* str;
		int i;
		unsigned int u;
		float f;
		bool b;
		char c;
		char h;
	};

	CustomGameSetting()
	{
		memset(this, 0, sizeof(CustomGameSetting));
	}

	~CustomGameSetting()
	{
	};

	void* __vftable;
	Info uValue;
	const char* pKey;

	void Initialize(const char* apName, Info afValue)
	{
		ThisCall(0x403100, this, apName, afValue);
	}

	void Initialize(const char* apName, float f)
	{
		Info i{};
		i.f = f;
		Initialize(apName, i);
	}

	void Initialize(const char* apName, UInt32 iv)
	{
		Info i{};
		i.i = iv;
		Initialize(apName, i);
	}

	void Initialize(const char* apName, bool b)
	{
		Info i{};
		i.b = b;
		Initialize(apName, i);
	}

	void Initialize(const char* apName, const char* s)
	{
		Info i{};
		i.str = s;
		Initialize(apName, i);
	}

	float Float() const
	{
		return uValue.f;
	}

	UInt32 Int() const
	{
		return uValue.i;
	}

	bool Bool() const
	{
		return uValue.b;
	}
};

namespace CustomGameSettings
{
	

	extern CustomGameSetting fDamageGunWeapCondBuffer;
	extern CustomGameSetting fDamageMeleeWeapCondBuffer;
	extern CustomGameSetting fArmorRatingCondBuffer;
	extern CustomGameSetting fPlayerGunshotVolumeMult;
	extern CustomGameSetting fNPCGunshotVolumeMult;
	extern CustomGameSetting fProjectileImpactVolumeMult;

	void Init();
};
