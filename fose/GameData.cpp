#include "GameData.h"


static const FormRecordData** s_FormRecordData = (const FormRecordData**)0x00F4A748;


DataHandler* DataHandler::Get() {
	return *g_dataHandler;
}

class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo) const
	{
		return _stricmp(modInfo->name, m_stringToFind) == 0;
	}
};

const ModInfo** DataHandler::GetActiveModList()
{
	static const ModInfo* activeModList[0x100] = {nullptr};

	if (!(*activeModList))
	{
		UInt16 index = 0;
		for (index = 0; index < Get()->modList.modInfoList.Count(); index++)
		{
			ModInfo* entry = Get()->modList.modInfoList.GetNthItem(index);
			if (entry->IsLoaded()) activeModList[index] = entry;
		}
	}

	return activeModList;
}

const char* DataHandler::GetNthModName(UInt32 modIndex)
{
	const ModInfo** activeModList = GetActiveModList();
	if (modIndex < GetActiveModCount() && activeModList[modIndex]) return activeModList[modIndex]->name;
	return "";
}

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}



UInt8 DataHandler::GetModIndex(const char* modName)
{
	return modList.modInfoList.GetIndexOf(LoadedModFinder(modName));
}

struct IsModLoaded
{
	bool Accept(ModInfo* pModInfo) const {
		return pModInfo->IsLoaded();
	}
};

UInt8 DataHandler::GetActiveModCount() const
{
	UInt32 count = modList.modInfoList.CountIf(IsModLoaded());
	return count;
}

//const char* DataHandler::GetNthModName(UInt32 modIndex)
//{
//	const ModEntry** activeModList = GetActiveModList();
//	if (modIndex < GetActiveModCount() && activeModList[modIndex]->data)
//		return activeModList[modIndex]->data->name;
//	else
//		return "";
//}


class QuestNameFinder
{
	const char* name;
public:
	QuestNameFinder(const char* _name) : name(_name)	{	}

	bool Accept(const TESQuest* quest)
	{
		if (!_stricmp(quest->GetEditorName(), name))
			return true;

		return false;
	}
};

TESQuest* DataHandler::GetQuestByName(const char* questName)
{
	return questList.Find(QuestNameFinder(questName));
}

ModInfo::ModInfo()
{
	//
}

ModInfo::~ModInfo()
{
	//
}
