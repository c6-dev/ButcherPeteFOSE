#pragma once
#include "GameForms.h"

class BSGameSound;
class BSWin32AudioListener;

struct BSSoundHandle
{
	SInt32 uiSoundID;
	bool bAssumeSuccess;
	UInt32 uiState;

	BSSoundHandle() : uiSoundID(-1), bAssumeSuccess(false), uiState(0)
	{
	}

	bool IsPlaying() const;
	bool Stop();
	bool Release();

	bool Play()
	{
		return ThisCall<bool>(0xBD00C0, this, 0);
	}

	UInt32 GetDuration()
	{
		return ThisCall<UInt32>(0xBD0270, this);
	}

	bool SetPosition(NiPoint3 akPosition)
	{
		return ThisCall<bool>(0xBD0290, this, akPosition);
	}

	void SetObjectToFollow(NiAVObject* apObject)
	{
		ThisCall(0xBD0470, this, apObject);
	}

	bool FadeInPlay(UInt32 auiMilliseconds)
	{
		return ThisCall<bool>(0xBD03E0, this, auiMilliseconds);
	}

	bool SetVolume(float volume)
	{
		return ThisCall<bool>(0xBD01C0, this, volume);
	}


	BSSoundHandle& operator=(BSSoundHandle other)
	{
		uiSoundID = other.uiSoundID;
		bAssumeSuccess = other.bAssumeSuccess;
		uiState = other.uiState;
		return *this;
	}
};

static_assert(sizeof(BSSoundHandle) == 0xC);

class BSWin32Audio
{
public:
	BSWin32Audio();
	~BSWin32Audio();

	virtual void Destroy(bool doFree);
	virtual void Unk_01(void);
	virtual void Unk_02(void);
	virtual void Unk_03(void);
	virtual void Unk_04(void);
	virtual BSGameSound* CreateGameSound(const char* filePath);
	virtual void Unk_06(void);
	virtual void Unk_07(void);

	UInt32 unk004[3]; // 004
	BSWin32AudioListener* listener; // 010
	UInt32 unk014[3]; // 014
	bool (*unk20)(UInt32*, UInt32*, UInt32*, UInt32*); // 020
	bool (*unk24)(UInt32*, UInt32*, UInt32*, UInt32*); // 024
	bool (*unk28)(UInt32*); // 028
	UInt32 (*unk2C)(UInt32*, TESSound*, UInt32*); // 02C
	void (*unk30)(void); // 030
	void (*unk34)(void); // 034

	static BSWin32Audio* GetSingleton()
	{
		return *(BSWin32Audio**)0x11790C8;
	};

	BSSoundHandle GetSoundHandleByFormID(UInt32 auiFormID, UInt32 aeAudioFlags)
	{
		BSSoundHandle kHandle;
		ThisCall(0xBCFBB0, this, &kHandle, auiFormID, aeAudioFlags);
		return kHandle;
	}
};
