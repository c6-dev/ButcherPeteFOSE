#pragma once

#include "GameTiles.h"
#include "GameInterface.h"
#include "GameObjects.h"
#include "SafeWrite.h"
#include "GameRTTI.h"
class BSGameSound;
class BSWin32AudioListener;
class BSWin32Audio
{
public:
	BSWin32Audio();
	~BSWin32Audio();

	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual BSGameSound* CreateGameSound(const char* filePath);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);

	UInt32					unk004[3];		// 004
	BSWin32AudioListener* listener;		// 010
	UInt32					unk014[3];		// 014
	bool					(*unk20)(UInt32*, UInt32*, UInt32*, UInt32*);	// 020
	bool					(*unk24)(UInt32*, UInt32*, UInt32*, UInt32*);	// 024
	bool					(*unk28)(UInt32*);	// 028
	UInt32(*unk2C)(UInt32*, TESSound*, UInt32*);	// 02C
	void					(*unk30)(void);	// 030
	void					(*unk34)(void);	// 034

	static BSWin32Audio* GetSingleton() { return *(BSWin32Audio**)0x11790C8; };
};

// 0C
struct Sound
{
	UInt32 soundKey;
	UInt8 byte04;
	UInt8 pad05[3];
	UInt32 unk08;

	Sound() : soundKey(0xFFFFFFFF), byte04(0), unk08(0) {}

	Sound(const char* soundPath, UInt32 flags)
	{
		ThisCall(0xBCFCD0, BSWin32Audio::GetSingleton(), this, soundPath, flags);
	}

	void Play()
	{
		ThisCall(0xBD00C0, this, 0);
	}

	UInt32 GetDuration() {
		return ThisCall<UInt32>(0xBD0270, this);
	}

	Sound& operator= (Sound other)
	{
		soundKey = other.soundKey;
		byte04 = other.byte04;
		unk08 = other.unk08;
		return *this;
	}
};
STATIC_ASSERT(sizeof(Sound) == 0xC);

enum MenuSpecialKeyboardInputCode
{
	kMenu_Enter = -2,
	kMenu_UpArrow = 0x1,
	kMenu_DownArrow = 0x2,
	kMenu_RightArrow = 0x3,
	kMenu_LeftArrow = 0x4,
	kMenu_Space = 0x9,
	kMenu_Tab = 0xA,
	kMenu_ShiftEnter = 0xB,
	kMenu_AltEnter = 0xC,
	kMenu_ShiftLeft = 0xD,
	kMenu_ShiftRight = 0xE,
	kMenu_PageUp = 0xF,
	kMenu_PageDown = 0x10,
};
// 28
class Menu
{
public:
	Menu() { ThisCall(0xBFCFE0, this); };
	~Menu() { ThisCall(0xBFD0F0, this); };

	enum vtable_offsets
	{
		kDestructor = 0,
		kSetTile = 4,
		kHandleLeftClickPress = 8,
		kHandleClick = 0xC,
		kHandleMouseover = 0x10,
		kHandleUnmouseover = 0x14,
		kPostDragTileChange = 0x18,
		kPreDragTileChange = 0x1C,
		kHandleActiveMenuClickHeld = 0x20,
		kOnClickHeld = 0x24,
		kHandleMousewheel = 0x28,
		kUpdate = 0x2C,
		kHandleKeyboardInput = 0x30,
		kGetID = 0x34,
		kHandleSpecialKeyInput = 0x38,
		kHandleControllerInput = 0x3C,
		kOnUpdateUserTrait = 0x40,
		kHandleControllerConnectOrDisconnect = 0x44
	};

	enum eVisibilityState : UInt32
	{
		kReady = 0x1,
		kFadeOut = 0x2,
		kInit = 0x4,
		kFadeIn = 0x8,
	};

	virtual void	Destructor(bool doFree) {};
	virtual void	SetTile(UInt32 tileID, Tile* value) {}; // can be used to determine how many Tile*s are in a Menu class
	virtual void	HandleLeftClickPress(UInt32 tileID, Tile* activeTile) {}; // called when the mouse has moved and left click is pressed
	virtual void	HandleClick(SInt32 tileID, Tile* clickedTile) {};
	virtual void	HandleMouseover(UInt32 tileID, Tile* activeTile) {};	//	Called on mouseover, activeTile is moused-over Tile
	virtual void	HandleUnmouseover(UInt32 tileID, Tile* tile) {};
	virtual void	PostDragTileChange(UInt32 tileID, Tile* newTile, Tile* activeTile) {}; // unused in vanilla
	virtual void	PreDragTileChange(UInt32 tileID, Tile* oldTile, Tile* activeTile) {}; // unused in vanilla
	virtual void	HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile) {}; // StartMenu, RaceSexMenu, VATSMenu, BookMenu
	virtual void	OnClickHeld(UInt32 tileID, Tile* activeTile) {}; // unused by vanilla menus
	virtual void	HandleMousewheel(UInt32 tileID, Tile* tile) {};
	virtual void	Update(void) {};	// Called every frame while the menu is active
	virtual bool	HandleKeyboardInput(UInt32 inputChar) { return false; };	// Return false for handling keyboard shortcuts
	virtual UInt32	GetID(void) { return 0; };
	virtual bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState) { return false; };
	virtual bool	HandleControllerInput(int input, Tile* tile) { return false; };
	virtual void    OnUpdateUserTrait(int tileVal) {}; // unimplemented by any vanilla menus, called at 0xA1F28E - called when a tilevalue > 10000 is used (any field starting with an underscore like <_XXX></_XXX> is a user trait)
	virtual void	HandleControllerConnectOrDisconnect(bool isControllerConnected) {};

	// 14
	struct TemplateInstance
	{
		UInt32		unk00;		// 00
		float		flt04;		// 04
		String		tileName;	// 08
		Tile* tile;		// 10
	};

	// 14
	struct TemplateData
	{
		const char* templateName;	// 00
		void* tileExtra;		// 04
		DList<TemplateInstance>	instances;		// 08
	};

	TileMenu* rootTile;			// 04
	tList<TemplateData>	menuTemplates;	// 08
	UInt32				unk10;			// 10
	UInt32				unk14;			// 14	Same as id (20)
	UInt32				unk18;			// 18
	UInt8				byte1C;			// 1C
	UInt8				byte1D;			// 1D
	UInt8				byte1E;			// 1E
	UInt8				byte1F;			// 1F
	UInt32				id;				// 20
	eVisibilityState	visibilityState;// 24 :: Initialised to 4
	// check 1 at 0xA0B174, 0x70D529, 0x70D592 :: set at 0x712224
	// check 2 at 0x711FF1 :: set 2 at 0xA1D987 (when closes menu), 0xA1DA41
	// check 4 at 0xA1D9EC (when closing menu) :: set at 0x7036A4, 0x71204D
	// check 8 at 0x712194 :: set 8 at 0xA1DB8F (when opening menu), 0x720B39

	Tile* AddTileFromTemplate(Tile* destTile, const char* templateName);
	void Close()
	{
		this->rootTile->SetFloat(6002, 1);
		ThisCall(0xBFC470, this);
	}

	void Free()
	{
		ThisCall(0xBFD0F0, this);
	}

	void RegisterTile(Tile* _tile, bool noAppendToMenuStack) { ThisCall(0xBFC570, this, _tile, noAppendToMenuStack); }
	void HideTitle(bool noFadeIn) { ThisCall(0xBFD010, this, noFadeIn); }
	void FadeOutAndClose() { ThisCall(0xBFC470, this); }
};

// 30
template <typename T>
class ListBox
{
public:
	ListBox();
	~ListBox();

	virtual bool	Unk_00(UInt32 arg0);
	virtual UInt32	GetUnk10(void);
	virtual void	Unk_02(UInt32 arg0);	// handles scrolling?
	virtual bool	Unk_03(UInt32 arg0);
	virtual void	Unk_04(void);
	virtual UInt32	Unk_05(UInt32 arg0, UInt32 arg1);
	virtual void	Destroy(bool noDealloc);
	virtual void	Unk_07(void);			// filter items? iterates over tList, frees selected nodes
	virtual void	Unk_08(UInt32 arg0);	// traverses tList twice

//	void			** _vtbl;		// 00
	tList<T>		list;			// 04 
	Tile			* listBoxTile;	// 0C
	UInt32			unk10;			// 10
	TileImage		* scrollbar;	// 14
	char			* templateName;	// 18
	UInt8			unk1C;			// 1C
	UInt8			pad1D[3];		// 1D
	float			unk20;			// 20
	float			unk24;			// 24
	float			unk28;			// 28
	UInt16			unk2C;			// 2C
	UInt16			pad2E;
};

STATIC_ASSERT(sizeof(ListBox<UInt32>) == 0x30);


enum {
	kMenuType_Audio                 =	0x000003F9,
	kMenuType_Book                  =	0x00000402,
	kMenuType_CharGen               =	0x00000418,
	kMenuType_Container             =	0x000003F0,
	kMenuType_Dialog                =	0x000003F1,
	kMenuType_Gameplay              =	0x000003FC,
	kMenuType_HUDMain               =	0x000003EC,
	kMenuType_Inventory             =	0x000003EA,
	kMenuType_Loading               =	0x000003EF,
	kMenuType_LockPick              =	0x000003F6,
	kMenuType_Map                   =	0x000003FF,
	kMenuType_Message               =	0x000003E9,
	kMenuType_Tutorial              =	0x00000423,
	kMenuType_TextEdit              =	0x0000041B,
	kMenuType_Start                 =	0x000003F5,
	kMenuType_Quantity              =	0x000003F8,
	kMenuType_RaceSex               =	0x0000040C,
	kMenuType_SleepWait             =	0x000003F4,
	kMenuType_Stats                 =	0x000003EB,
	kMenuType_Video                 =	0x000003FA,
	kMenuType_LevelUp               =	0x00000403,
	kMenuType_Repair                =	0x0000040B,
	kMenuType_RepairServices        =	0x00000422,
	kMenuType_Credits               =	0x00000417,
	kMenuType_Barter                =	0x0000041D,
	kMenuType_Surgery               =	0x0000041E,
	kMenuType_Hacking               =	0x0000041F,
	kMenuType_Computers             =	0x00000421,
	kMenuType_VATS                  =	0x00000420,
	kMenuType_SPECIALBook           =	0x00000424,

	kMenuType_Min = kMenuType_Message,
	kMenuType_Max = kMenuType_SPECIALBook
};



struct DialogueResponse
{
	String responseText; // 0
	UInt32 emotionType;	 // 8
	UInt32* emotionValue; // C
	String voiceFilePath;// 10
	TESIdleForm* speakerAnimation;	// 18
	TESIdleForm* listenerAnimation;	// 1C
	UInt32 soundID;	// 20
	UInt8 flags;	// 24
	UInt8 gap25[3];
	UInt32 responseNumber; // 28
};

struct DialogueResponseList
{
	ListNode<DialogueResponse> list;
	ListNode<DialogueResponse>* current;

	bool Next()
	{
		if (current)
		{
			current = current->next;
		}
		return current != nullptr;
	}
};
struct DialogueItem
{
	DialogueResponseList responses; // 00
	TESTopicInfo* topicInfo;// 0C
	TESTopic* topic;		// 10
	TESQuest* quest;		// 14
	Actor* speaker;			// 18


};
STATIC_ASSERT(sizeof(DialogueItem) == 0x1C);