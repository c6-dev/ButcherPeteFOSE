#include "NewConditionFunctions.hpp"

#include "CommandTable.h"
#include "SafeWrite.h"
#include "Utilities.h"

// Eval stub for the editor, indicates that this is a condition function. Same as NVSE
bool __cdecl DefaultEval(COMMAND_ARGS_EVAL)
{
	return true;
}

namespace NewConditionFunctions
{
	// fixes the condition function dropdown population, which truncates the opcode, causing the selection to reset for new functions
	int __fastcall ConditionListOpcodeHook(UInt32 ECX, UInt32 EDX, UInt32 opCode)
	{
		_asm {
			mov eax, opCode
			add eax, 0x01000
			push eax
			mov eax, 0x4A59A0
			call eax
			}
	}

	// restores the eval pointer for condition functions in the editor command table, which FOSE sets to null
	// only the known ButcherPete and Command Extender opcodes are defined, since there is no easy way of identifying valid condition functions
	// with the eval pointer being set to null. Other opcodes can be added, provided that the source plugin defines the _Eval for use at runtime.
	void FixConditionFunctions()
	{
		UInt32 startAddr = *(UInt32*)0x00406D4C;
		UInt32 endAddr = *(UInt32*)0x004A594D;
		auto start = reinterpret_cast<const CommandInfo*>(startAddr);
		auto end = reinterpret_cast<const CommandInfo*>(endAddr);
		gLog.Message("Registering condition functions in GECK:\n");
		for (const CommandInfo* it = start; it < end; ++it)
		{
			if (!it) continue;
			auto writable = const_cast<CommandInfo*>(it);
			if (!writable) continue;
			switch (writable->opcode)
			{
			case 0x2220: // IsOwned
			case 0x2224: // GetPCCanFastTravel
			case 0x2225: // GetRadiationLevelAlt
			case 0x222D: // GetLockedAlt
			case 0x2239: // GetIsRagdolled
			case 0x2258: // IsInWater
			case 0x225C: // IsIdlePlayingEx
			case 0x226B: // IsSpellTargetAlt
			case 0x22A0: // IsNight
			case 0x22A8: // GetQuestFailedAlt
			case 0x3117: // GetGameDifficulty (CE)
			case 0x3119: // GetAlwaysRun (CE)
			case 0x311A: // GetAutoMove (CE)
			case 0x3126: // GetWaterImmersionPerc (CE)
			case 0x3163: // IsButtonPressed (CE)
			case 0x3164: // GetLeftStickX (CE)
			case 0x3165: // GetLeftStickY (CE)
			case 0x3166: // GetRightStickX (CE)
			case 0x3167: // GetRightStickY (CE)
			case 0x3168: // GetLeftTrigger (CE)
			case 0x3169: // GetRightTrigger (CE)
			case 0x317B: // IsRefInList (CE)
			case 0x317F: // GetNoteRead (CE)
			case 0x3195: // IsInAir (CE)
			case 0x31A2: // IsButtonDisabled (CE)
			case 0x31A5: // IsButtonHeld (CE)
			case 0x31A8: // IsTriggerDisabled (CE)
			case 0x31AB: // IsTriggerHeld (CE)
				gLog.FormattedMessage("%s (0x%X)", writable->longName, writable->opcode);
				writable->eval = DefaultEval;
				break;
			default:
				break;
			}
		}

		return StdCall(0x51E730);
	}

	void Init()
	{
		WriteRelCall(0x4A6B04, (UInt32)ConditionListOpcodeHook);
	}

	void DeferredInit()
	{
		FixConditionFunctions();
	}
}
