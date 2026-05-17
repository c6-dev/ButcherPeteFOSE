#pragma once

#include <span>
#include "GameForms.h"
#include "CommandTable.h"
#include <vector>
struct ScriptEventList;
struct ScriptBuffer;
struct ScriptOperator;

extern std::span<CommandInfo> g_eventBlockCommandInfos;
extern std::span<CommandInfo> g_scriptStatementCommandInfos;
extern std::span<ScriptOperator> g_gameScriptOperators;
extern std::span<ActorValueInfo*> g_actorValues;

#if RUNTIME
#define SCRIPT_SIZE 0x54
#elif EDITOR
#define SCRIPT_SIZE 0x48
#endif


struct ScriptLocal;

struct VariableInfo
{
	UInt32 idx; // 00
	UInt32 pad04; // 04
	double data; // 08
	UInt8 type; // 10
	UInt8 pad11[3]; // 11
	UInt32 unk14; // 14
	String name; // 18

	ScriptLocal* Resolve(ScriptEventList* eventList);
	TESForm* GetTESForm();
	bool IsReferenceType(Script* parentScript);
};

// 54 / 48
class Script : public TESForm
{
public:
	Script();
	~Script();
	MEMBER_FN_PREFIX(Script);
	struct RefVariable
	{
		String name; // 000 variable name/editorID (not used at run-time)
		TESForm* form; // 008
		UInt32 varIdx; // 00C


		Script* GetReferencedScript() const;
		void Resolve(ScriptEventList* eventList);
	};

	struct RefList : tList<RefVariable>
	{
		UInt32 GetIndex(RefVariable* refVar);
	};

	enum VariableType : UInt8
	{
		eVarType_Float = 0, // ref is also zero
		eVarType_Integer,

		// NVSE, return values only
		eVarType_String,
		eVarType_Array,
		eVarType_Ref,

		eVarType_Invalid
	};

	struct VarInfoList : tList<VariableInfo>
	{
		VariableInfo* GetVariableByName(const char* name);
	};


	// 14
	struct ScriptInfo
	{
		UInt32 unusedVariableCount; // 00 (18)
		UInt32 numRefs; // 04 (1C)
		UInt32 dataLength; // 08 (20)
		UInt32 varCount; // 0C (24)
		UInt16 type; // 10 (28)
		bool compiled; // 12 (2A)
		UInt8 unk13; // 13 (2B)
	};

	enum
	{
		eType_Object = 0,
		eType_Quest = 1,
		eType_Magic = 0x100,
		eType_Unk = 0x10000,
	};
#if !RUNTIME
	UInt32 unk028; //     /     / 028
#endif
	ScriptInfo info; // 018 / 018 / 02C
	char* text; // 02C / 02C / 040
	UInt8* data; // 030 / 030 / 044
#if RUNTIME
	float unk34; // 034
	float questDelayTimeCounter; // 038      - init'd to fQuestDelayTime, decremented by frametime each frame
	float secondsPassed; // 03C      - only if you've modified fQuestDelayTime
	TESQuest* quest; // 040
#endif
	RefList refList; // 044 / 034 / 048 - ref variables and immediates
	VarInfoList varList; // 04C / 03C / 050 - local variable list
#if !RUNTIME
	void* unk050; //     /     / 050
	UInt8 unk054; //	   /     / 054
	UInt8 pad055[3];
#endif

	RefVariable* GetRefFromRefList(UInt32 refIdx);
	VariableInfo* GetVariableInfo(UInt32 idx);
	VariableInfo* GetVariableByName(const char* varName);

	RefVariable* GetVariable(UInt32 reqIdx);


	UInt32 Type() const
	{
		return info.type;
	}

	bool IsObjectScript() const
	{
		return info.type == eType_Object;
	}

	bool IsQuestScript() const
	{
		return info.type == eType_Quest;
	}

	bool IsMagicScript() const
	{
		return info.type == eType_Magic;
	}

	bool IsUnkScript() const
	{
		return info.type == eType_Unk;
	}


	ScriptEventList* CreateEventList();
};

static_assert(sizeof(Script) == SCRIPT_SIZE);

struct ScriptRefVariable : Script::RefVariable
{
};

struct ConditionEntry
{
	struct Data
	{
		union Param
		{
			float number;
			TESForm* form;
		};

		// ### TODO: this
		UInt32 operatorAndFlags; // 00
		float comparisonValue; // 04
		UInt16 functionIndex; // 08 is opcode & 0x0FFF
		UInt16 unk0A;
		Param param1; // 0C
		Param param2; // 10
		UInt32 unk14;
	};

	Data* data;
	ConditionEntry* next;
};

// 6C
struct QuestStageItem
{
	uint8_t uiFlags;
	ConditionList kConditions;
	Script kScript;
	UInt32 uiFileOffset;
	uint8_t ucLogEntryIndex;
	bool bHasLogEntry;
	void* pCompletionDate;
	TESQuest* pOwner;
	TESQuest* pNextQuest;
};

#if RUNTIME
static_assert(sizeof(QuestStageItem) == (SCRIPT_SIZE + 0x20));
#endif


Script::VariableType VariableTypeNameToType(const char* name);
const char* VariableTypeToName(Script::VariableType type);

Script::VariableType GetDeclaredVariableType(const char* varName, const char* scriptText, Script* script);
// parses scriptText to determine var type
Script* GetScriptFromForm(TESForm* form);

bool GetUserFunctionParamTokensFromLine(std::string_view lineText, std::vector<std::string>& out);


enum class ScriptOperatorCode
{
	kOp_LeftBracket = 0x0,
	kOp_RightBracket = 0x1,
	kOp_LogicalAnd = 0x2,
	kOp_LogicalOr = 0x3,
	kOp_LessThanOrEqual = 0x4,
	kOp_LessThan = 0x5,
	kOp_GreaterThanOrEqual = 0x6,
	kOp_GreaterThan = 0x7,
	kOp_Equals = 0x8,
	kOp_NotEquals = 0x9,
	kOp_Minus = 0xA,
	kOp_Plus = 0xB,
	kOp_Multiply = 0xC,
	kOp_Divide = 0xD,
	kOp_Modulo = 0xE,
	kOp_Tilde = 0xF,
	kOp_MAX = 0x10,
};

struct ScriptOperator
{
	ScriptOperatorCode code;
	UInt8 precedence;
	char operatorString[3];
};

// 78
struct TerminalEntry
{
	String entryText; // 00
	String resultText; // 08
	Script resultScript; // 10
	ConditionEntry conditions; // 64
	BGSNote* displayNote; // 6C
	BGSTerminal* subMenu; // 70
	UInt8 byte74; // 74
	UInt8 pad75[3]; // 75
};

static_assert(sizeof(TerminalEntry) == 0x78);
