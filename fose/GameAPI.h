#pragma once

#include "CommandTable.h"
#include "GameTypes.h"
#include "Utilities.h"

class TESObjectREFR;
class Script;
class TESForm;
class SceneGraph;
class Tile;
class DataHandler;
class ChangesMap;
class InteriorCellNewReferencesMap;
class ExteriorCellNewReferencesMap;
class NumericIDBufferMap;

// only records individual objects if there's a block that matches it
// ### how can it tell?
struct ScriptEventList
{
	struct Event
	{
		TESForm	* object;
		UInt32	eventMask;
	};
	typedef tList<Event> EventList;
	struct VarEntry;

	struct Var
	{
		UInt32		id;
		VarEntry	* nextEntry;
		double		data;
	};

	struct VarEntry
	{
		Var			* var;
		VarEntry	* next;
	};

	Script			* m_script;
	UInt32			m_unk1;
	EventList	* m_eventList;
	VarEntry		* m_vars;

	void	Dump(void);
	Var *	GetVariable(UInt32 id);
	UInt32	ResetAllVariables();
};

#if RUNTIME



const UInt32 _ConsoleManager_Print = 0x0062B190;

#else

const UInt32 _ConsoleManager_Print = 0;

#endif

void Console_Print(const char * fmt, ...);

typedef bool (* _ExtractArgs)(ParamInfo * paramInfo, void * arg1, UInt32 * arg2, TESObjectREFR * arg3, TESObjectREFR * arg4, Script * script, ScriptEventList * eventList, ...);
extern const _ExtractArgs ExtractArgs;

typedef void * (* _FormHeap_Allocate)(UInt32 size);
extern const _FormHeap_Allocate FormHeap_Allocate;

typedef void (* _FormHeap_Free)(void * ptr);
extern const _FormHeap_Free FormHeap_Free;

typedef TESForm * (* _LookupFormByID)(UInt32 id);
extern const _LookupFormByID LookupFormByID;

typedef TESForm * (* _CreateFormInstance)(UInt8 type);
extern const _CreateFormInstance CreateFormInstance;

const char * GetFullName(TESForm * baseForm);
const char* GetActorValueString(UInt32 actorValue); // not working yet

typedef char* (* _GetActorValueName)(UInt32 actorValueCode);
extern const _GetActorValueName GetActorValueName;

UInt32 AddFormToDataHandler(TESForm * form);
extern DataHandler ** g_dataHandler;

#if RUNTIME

	const UInt32 _TESSaveLoadGame_AddCreatedForm			= 0x006DD260;
	const UInt32 _SaveGameManager_ConstructSavegameFilename = 0x006D47A0;
	const UInt32 _SaveGameManager_ConstructSavegamePath		= 0x006D46E0;

#else
	const UInt32 _TESSaveLoadGame_AddCreatedForm = 0;
	const UInt32 _SaveGameManager_ConstructSavegameFilename = 0;
	const UInt32 _SaveGameManager_ConstructSavegamePath = 0;
#endif

// 1C8
class TESSaveLoadGame
{
public:
	TESSaveLoadGame();
	~TESSaveLoadGame();

	MEMBER_FN_PREFIX(TESSaveLoadGame);
	DEFINE_MEMBER_FN(AddCreatedForm, UInt32, _TESSaveLoadGame_AddCreatedForm, TESForm* pForm);
	static TESSaveLoadGame* GetSingleton();

	struct CreatedObject {
		UInt32			refID;
		CreatedObject	* next;
	};

	ChangesMap					* changesMap;		// 000
	UInt32						unk004;				// 004
	InteriorCellNewReferencesMap	* intRefMap;	// 008
	ExteriorCellNewReferencesMap	* extRefMap00C;	// 00C
	ExteriorCellNewReferencesMap	* extRefMap010;	// 010
	void						* saveLoadBuffer;	// 014
	UInt32						unk018;				// 018
	UInt8						unk01C;				// 01C
	UInt8						pad01D[3];
	UInt32						unk020;				// 020
	UInt32						unk024;				// 024
	UInt32						unk028;				// 028
	CreatedObject				createdObjectList;	// 02C data is formID
	UInt32				unk034[(0x58-0x34) >> 2];	// 034
	NumericIDBufferMap			* idMap058;			// 058
	NumericIDBufferMap			* idMap05C;			// 05C
	NumericIDBufferMap			* idMap060;			// 060
	NumericIDBufferMap			* idMap064;			// 064
	UInt32						unk068;				// 068
	UInt32						unk06C;				// 06C
	UInt32						unk070;				// 070
	UInt8						unk074;				// 074
	UInt8						unk075;				//     init to 0x7D
	UInt8						pad076[2];
	NiTArray<UInt32>			* array078;			// 078 NiTLargePrimitiveArray<?>
	NiTArray<UInt32>			* array07C;			// 07C NiTLargePrimitiveArray<?>	
	UInt8						unk080;				// 080 version of save?
	UInt8						unk081;
	UInt8						pad082[2];
	UInt32				unk084[(0xAC-0x84) >> 2];	// 084
	UInt8						unk0AC;				// 0AC
	UInt8						unk0AD;				// 0AD
	UInt8						unk0AE;				// 0AE
	UInt8						unk0AF;				// 0AF
	UInt32				unk0B0[(0x1C8-0x0B0) >> 2];	// 0B0
};

STATIC_ASSERT(sizeof(TESSaveLoadGame) == 0x1C8);

UInt32 AddFormToCreatedBaseObjectsList(TESForm * form);
//extern TESSaveLoadGame ** g_saveLoadGame;

// 2C
class SaveGameManager
{
public:
	SaveGameManager();
	~SaveGameManager();

	static SaveGameManager* GetSingleton();
	MEMBER_FN_PREFIX(SaveGameManager);
	DEFINE_MEMBER_FN(ConstructSavegameFilename, void, _SaveGameManager_ConstructSavegameFilename, 
					 const char* filename, char* outputBuf, bool bTempFile);
	DEFINE_MEMBER_FN(ConstructSavegamePath, void, _SaveGameManager_ConstructSavegamePath, char* outputBuf);

	// size unknown
	struct SaveGameData
	{
		const char	* name;		// 00
		UInt32		unk04;		// 04
		UInt32		unk08;		// 08 index?
		const char	* pcName;	// 0C
		const char	* pcTitle;	// 10
		const char	* location;	// 14
		const char	* time;		// 18
	};

	tList<SaveGameData>		* saveList;		// 00
	UInt32					numSaves;		// 04
	UInt32					unk08;			// 08
	UInt8					unk0C;			// 0C
	UInt8					unk0D;
	UInt8					unk0E;
	UInt8					unk0F;
	const char				* unk10;		// 10 name of most recently loaded/saved game?
	UInt32					unk14;			// 14 init to -1
	UInt8					unk18;			// 18
	UInt8					pad19[3];
	UInt8					unk20;			// 20 init to 1
	UInt8					unk21;
	UInt8					pad22[2];
	UInt32					unk24;			// 24
	UInt32					unk28;			// 28
};

std::string GetSavegamePath();

class ConsoleManager
{
public:
	MEMBER_FN_PREFIX(ConsoleManager);
	DEFINE_MEMBER_FN(Print, void, _ConsoleManager_Print, const char * fmt, va_list args);

	ConsoleManager();
	~ConsoleManager();

	static ConsoleManager * GetSingleton(void);
};

bool IsConsoleMode();
void DumpTLSData();

const UInt32 kMaxMessageLength = 0x4000;

bool ExtractArgsEx(ParamInfo * paramInfo, void * scriptData, UInt32 * scriptDataOffset, Script * scriptObj, ScriptEventList * eventList, ...);
extern bool ExtractFormatStringArgs(UInt32 fmtStringPos, char* buffer, ParamInfo * paramInfo, void * scriptDataIn, UInt32 * scriptDataOffset, Script * scriptObj, ScriptEventList * eventList, UInt32 maxParams, ...);

// A plugin author requested the ability to use OBSE format specifiers to format strings with the args
// coming from a source other than script.
// So changed ExtractFormattedString to take an object derived from following class, containing the args
// Probably doesn't belong in GameAPI.h but utilizes a bunch of stuff defined here and can't think of a better place for it
class FormatStringArgs
{
public:
	enum argType {
		kArgType_Float,
		kArgType_Form		// TESForm*
	};

	virtual bool Arg(argType asType, void * outResult) = 0;	// retrieve next arg
	virtual bool SkipArgs(UInt32 numToSkip) = 0;			// skip specified # of args
	virtual bool HasMoreArgs() = 0;
	virtual std::string GetFormatString() = 0;						// return format string
};

// concrete class used for extracting script args
class ScriptFormatStringArgs : public FormatStringArgs
{
public:
	virtual bool Arg(argType asType, void* outResult);
	virtual bool SkipArgs(UInt32 numToSkip);
	virtual bool HasMoreArgs();
	virtual std::string GetFormatString();

	ScriptFormatStringArgs(UInt32 _numArgs, UInt8* _scriptData, Script* _scriptObj, ScriptEventList* _eventList);
	UInt32 GetNumArgs();
	UInt8* GetScriptData();

private:
	UInt32			numArgs;
	UInt8			* scriptData;
	Script			* scriptObj;
	ScriptEventList	* eventList;
	std::string fmtString;
};

enum EActorVals {
	eActorVal_Aggression			= 0,
	eActorVal_Confidence			= 1,
	eActorVal_Energy				= 2,
	eActorVal_Responsibility		= 3,
	eActorVal_Mood					= 4,

	eActorVal_Strength				= 5,
	eActorVal_Perception			= 6,
	eActorVal_Endurance				= 7,
	eActorVal_Charisma				= 8,
	eActorVal_Intelligence			= 9,
	eActorVal_Agility				= 10,
	eActorVal_Luck					= 11,
	eActorVal_SpecialStart = eActorVal_Strength,
	eActorVal_SpecialEnd = eActorVal_Luck,

	eActorVal_ActionPoints			= 12,
	eActorVal_CarryWeight			= 13,
	eActorVal_CritChance			= 14,
	eActorVal_HealRate				= 15,
	eActorVal_Health				= 16,
	eActorVal_MeleeDamage			= 17,
	eActorVal_DamageResistance		= 18,
	eActorVal_PoisonResistance		= 19,
	eActorVal_RadResistance			= 20,
	eActorVal_SpeedMultiplier		= 21,
	eActorVal_Fatigue				= 22,
	eActorVal_Karma					= 23,
	eActorVal_XP					= 24,

	eActorVal_Head					= 25,
	eActorVal_Torso					= 26,
	eActorVal_LeftArm				= 27,
	eActorVal_RightArm				= 28,
	eActorVal_LeftLeg				= 29,
	eActorVal_RightLeg				= 30,
	eActorVal_Brain					= 31,
	eActorVal_BodyPartStart = eActorVal_Head,
	eActorVal_BodyPartEnd = eActorVal_Brain,

	eActorVal_Barter				= 32,
	eActorVal_BigGuns				= 33,
	eActorVal_EnergyWeapons			= 34,
	eActorVal_Explosives			= 35,
	eActorVal_Lockpick				= 36,
	eActorVal_Medicine				= 37,
	eActorVal_MeleeWeapons			= 38,
	eActorVal_Repair				= 39,
	eActorVal_Science				= 40,
	eActorVal_SmallGuns				= 41,
	eActorVal_Sneak					= 42,
	eActorVal_Speech				= 43,
	eActorVal_Throwing				= 44,
	eActorVal_Unarmed				= 45,
	eActorVal_SkillsStart = eActorVal_Barter,
	eActorVal_SkillsEnd = eActorVal_Unarmed,

	eActorVal_InventoryWeight		= 46,
	eActorVal_Paralysis				= 47,
	eActorVal_Invisibility			= 48,
	eActorVal_Chameleon				= 49,
	eActorVal_NightEye				= 50,
	eActorVal_DetectLifeRange		= 51,
	eActorVal_FireResistance		= 52,
	eActorVal_WaterBreathing		= 53,
	eActorVal_RadLevel				= 54,
	eActorVal_BloodyMess			= 55,
	eActorVal_UnarmedDamage			= 56,
	eActorVal_Assistance			= 57,

	eActorVal_ElectricResistance	= 58,
	eActorVal_FrostResistance		= 59,
	eActorVal_EnergyResistance		= 60,
	eActorVal_EMPResistance			= 61,

	eActorVal_Var1Medical			= 62,
	eActorVal_Var2					= 63,
	eActorVal_Var3					= 64,
	eActorVal_Var4					= 65,
	eActorVal_Var5					= 66,
	eActorVal_Var6					= 67,
	eActorVal_Var7					= 68,
	eActorVal_Var8					= 69,
	eActorVal_Var9					= 70,
	eActorVal_Var10					= 71,

	eActorVal_IgnoreCrippledLimbs	= 72,
	eActorVal_FalloutMax = eActorVal_IgnoreCrippledLimbs,
};


class BGSSaveLoadGame	// 0x011DDF38
{
public:
	BGSSaveLoadGame();
	~BGSSaveLoadGame();

	typedef UInt32	RefID;
	typedef UInt32	IndexRefID;

	struct SaveChapters	// 06E	chapter table in save
	{
		struct RefIDArray	// List of all refID referenced in save for tranlation in BGSSaveLoadFormIDMap
		{
			UInt32	count;	// 000
			RefID	IDs[1];	// 004
		};

		RefIDArray* arr000;	// 000
		RefIDArray* arr004;	// 004

	};

	struct BGSSaveLoadReferencesMap
	{
		NiTPointerMap<UInt32>* map000;	// 000
		void* map010;	// 010
		NiTPointerMap<void*>* map020;	// 020
	};

	void* changesMap;			// 000
	void* previousChangeMap;	// 004
	void* refIDmapping;			// 008
	void* visitedWorldspaces;	// 00C
	BGSSaveLoadReferencesMap* referencesMap;		// 010
	void* QueuedSubBuffersMap;	// 014
	void* ChangedFormIDMap;		// 018
	char** saveGameHistory;		// 01C
	void* rfiafMap;				// 020
	BSSimpleArray<void*>		changedForms;			// 024
	NiTPointerMap<void*>					QueuedInitPackageLocationsActorMap;	// 034
	UInt8									saveMods[255];			// 044
	UInt8									loadedMods[255];		// 143
	UInt16									pad242;					// 242
	UInt32									iGlobalFlags;			// 244 bit 6 block updating player position/rotation from save, bit 2 set during save
	UInt8									formVersion;			// 248
	UInt8									pad249[3];				// 249

	static BGSSaveLoadGame* GetSingleton() { return *(BGSSaveLoadGame**)0x1079858; };
	bool IsLoading() { return (iGlobalFlags & 2) != 0; };
};
STATIC_ASSERT(sizeof(BGSSaveLoadGame) == 0x24C);

struct ProcessManager
{
	struct ProcessLists
	{
		NiTArray<void*>	objects;			// 000
		UInt32					beginOffsets[4];	// 010	0: High, 1: Mid-High, 2: Mid-Low, 3: Low
		UInt32					endOffsets[4];		// 020
		UInt32					unk034[4];			// 030
	};

	float					time000;			// 000
	ProcessLists			processLists;		// 004
	tList<void>* crimes[5];			// 044
	tList<void>				list058;			// 058
	tList<void>		tempEffects;		// 060
	UInt32					unk068[6];			// 068
	tList<void>			highActors;			// 080
	void* nearbyActorsArray[50]; // 088
	UInt32					numNearbyActors;	// 150
	float fCommentOnPlayerActionsTimer;			// 154
	float fCommentOnPlayerKnockingThingsTimer;	// 158
	UInt8 playerInRadiation1;					// 15C
	UInt8 playerInRadiation2;					// 15D
	UInt8 gap15E[2];							// 15E
	static ProcessManager* GetSingleton() { return (ProcessManager*)0x107C0A0; };
};
STATIC_ASSERT(sizeof(ProcessManager) == 0x160);