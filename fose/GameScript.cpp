#include "GameAPI.h"
#include "GameScript.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "CommandTable.h"
#include "GameRTTI.h"
#include <GameExtraData.h>


#if RUNTIME
std::span<CommandInfo> g_eventBlockCommandInfos = {reinterpret_cast<CommandInfo*>(0xF525D0), 36};
std::span<CommandInfo> g_scriptStatementCommandInfos = {reinterpret_cast<CommandInfo*>(0xF510E0), 16};
std::span<ScriptOperator> g_gameScriptOperators = {reinterpret_cast<ScriptOperator*>(0xF51060), 16};
std::span<ActorValueInfo*> g_actorValues = {reinterpret_cast<ActorValueInfo**>(0x1073EB8), eActorVal_FalloutMax};
#endif

#if RUNTIME

ScriptEventList* Script::CreateEventList(void)
{
	return ThisCall<ScriptEventList*>(0x5195C0, this);
}

#endif

Script::RefVariable* Script::GetRefFromRefList(UInt32 refIdx)
{
	UInt32 idx = 1; // yes, really starts at 1
	if (refIdx)
	{
		auto* varIter = refList.Head();
		do
		{
			if (idx == refIdx) return varIter->data;
			idx++;
		}
		while (varIter = varIter->next);
	}
	return nullptr;
}

VariableInfo* Script::GetVariableInfo(UInt32 idx)
{
	for (auto* entry = varList.Head(); entry; entry = entry->next)
		if (entry->data && entry->data->idx == idx) return entry->data;

	return nullptr;
}


#if RUNTIME
Script* Script::RefVariable::GetReferencedScript() const
{
	if (!form) return nullptr;
	if (IS_ID(form, Quest)) return static_cast<TESQuest*>(form)->scriptable.script;
	if (auto* refr = DYNAMIC_CAST(form, TESForm, TESObjectREFR))
	{
		BSExtraData* xData = refr->extraDataList.GetByType(kExtraData_Script);
		if (xData)
		{
			const auto xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript);
			if (xScript) return xScript->script;
		}
	}
	return nullptr;
}
#endif

class ScriptVarFinder
{
public:
	const char* m_varName;

	ScriptVarFinder(const char* varName) : m_varName(varName)
	{
	}

	bool Accept(VariableInfo* varInfo)
	{
		//_MESSAGE("  cur var: %s to match: %s", varInfo->name.m_data, m_varName);
		if (!_stricmp(m_varName, varInfo->name.m_data)) return true;
		return false;
	}
};

VariableInfo* Script::GetVariableByName(const char* varName)
{
	VariableInfo* pVariableInfo = varList.Find(ScriptVarFinder(varName));
	return pVariableInfo;
}

Script::RefVariable* Script::GetVariable(UInt32 reqIdx)
{
	if (reqIdx > 0) reqIdx--;
	RefVariable* pRefVar = refList.GetNthItem(reqIdx);
	return pRefVar;
}

void Script::RefVariable::Resolve(ScriptEventList* eventList)
{
	if (varIdx && eventList)
	{
		ScriptEventList::Var* var = eventList->GetVariable(varIdx);
		if (var)
		{
			UInt32 refID = *((UInt32*)&var->data);
			form = LookupFormByID(refID);
		}
	}
}

bool VariableInfo::IsReferenceType(Script* parentScript)
{
	return std::find_if(parentScript->refList.begin(), parentScript->refList.end(), [&](auto it)
	{
		return it && it->varIdx == this->idx;
	}) != parentScript->refList.end();
}
