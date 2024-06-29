#pragma once

// keeping this in a separate file so we don't need to include dinput/dsound everywhere

#define DIRECTINPUT_VERSION 0x0800
#define DIRECTSOUND_VERSION 0x0800
#include <dinput.h>
//#include <dsound.h>

// 1C00 (1.0) / 1C04 (1.1)
class OSInputGlobals
{
public:
	enum
	{
		kFlag_HasJoysticks =	1 << 0,
		kFlag_HasMouse =		1 << 1,
		kFlag_HasKeyboard =		1 << 2,
		kFlag_BackgroundMouse =	1 << 3,
	};

	enum
	{
		kMaxControlBinds = 0x1C,
	};



	UInt32		unk0000;							// 0000
	UInt32		flags;								// 0004
	void		* unk0008;							// 0008 IDirectInput8* ?
	UInt32		unk000C;							// 000C
	UInt32		unk0010;							// 0010
	UInt32		unk0014;							// 0014
	UInt32		unk0018;							// 0018
	UInt32		unk001C;							// 001C
	UInt32		unk0020;							// 0020
	UInt32		unk0024;							// 0024
	UInt32		unk0028;							// 0028
	void		* unk002C;							// 002C
	void		* unk0030;							// 0030
	UInt32		unk0034[(0x1A30 - 0x0034) >> 2];	// 0034
	UInt32		unk1A30;							// 1A30
	UInt32		unk1A34[(0x1AF8 - 0x1A34) >> 2];	// 1A34
	UInt32		unk1AF8;							// 1AF8
	UInt32		unk1AFC;							// 1AFC
	UInt32		unk1B00;							// 1B00
	UInt32		unk1B04;							// 1B04
	UInt32		unk1B08;							// 1B08
	UInt32		unk1B0C[(0x1B50 - 0x1B0C) >> 2];	// 1B0C
	UInt32		unk1B50;							// 4 bytes added between +2C and here for v1.1.0.35
	UInt32		oldDoubleClickTime;					// 1B50 (1.0) / 1B54 (1.1)
	UInt32		unk1B54[(0x1B90 - 0x1B54) >> 2];	// 1B54 / 1B58
	UInt8		keyBinds[kMaxControlBinds];			// 1B90 / 1B94
	UInt8		mouseBinds[kMaxControlBinds];		// 1BAC / 1BB0
	UInt8		joystickBinds[kMaxControlBinds];	// 1BC8 / 1BCC
	UInt32		unk1BE4[(0x1C00 - 0x1BE4) >> 2];	// 1BE4 / 1BE8
};


STATIC_ASSERT(sizeof(OSInputGlobals) == 0x1C04);
STATIC_ASSERT(offsetof(OSInputGlobals, mouseBinds) == 0x1BB0);



extern OSInputGlobals** g_OSInputGlobals;


class OSGlobals
{
};
extern OSGlobals ** g_osGlobals;

