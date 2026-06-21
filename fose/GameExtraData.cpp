#include "GameExtraData.h"
#include "GameRTTI.h"


ExtraContainerChanges::ExtendDataList* ExtraContainerChangesExtendDataListCreate(ExtraDataList* pExtraDataList)
{
	ExtraContainerChanges::ExtendDataList* xData = (ExtraContainerChanges::ExtendDataList*)GameHeapAlloc(sizeof(ExtraContainerChanges::ExtendDataList));
	xData->Init();
	if (pExtraDataList) xData->Append(pExtraDataList);
	return xData;
}


ExtraContainerChanges* ExtraContainerChanges::Create()
{
	UInt32* dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraContainerChanges));
	dataPtr[0] = 0xDB6F90;
	dataPtr[1] = kExtraData_ContainerChanges;
	dataPtr[2] = 0;
	dataPtr[3] = 0;
	return (ExtraContainerChanges*)dataPtr;
}

ExtraContainerChanges::Data* ExtraContainerChanges::Data::Create(TESObjectREFR* owner)
{
	Data* data = (Data*)GameHeapAlloc(sizeof(Data));
	memset(data, 0, sizeof(Data));
	data->owner = owner;
	data->objList = (EntryDataList*)GameHeapAlloc(sizeof(EntryDataList));
	data->objList->Init();
	return data;
}


// static
BSExtraData* BSExtraData::Create(UInt8 xType, UInt32 size, UInt32 vtbl)
{
	void* memory = FormHeap_Allocate(size);
	memset(memory, 0, size);
	((UInt32*)memory)[0] = vtbl;
	BSExtraData* xData = (BSExtraData*)memory;
	xData->type = xType;
	return xData;
}

ExtraContainerChanges::ExtendDataList* ExtraContainerChanges::EntryData::Add(ExtraDataList* newList)
{
	if (extendData)
		extendData->AddAt(newList, eListEnd);
	else
		/* ExtendDataList* */ extendData = ExtraContainerChangesExtendDataListCreate(newList);
	ExtraCount* xCount = (ExtraCount*)newList->GetByType(kExtraData_Count);
	countDelta += xCount ? xCount->count : 1;
	return extendData;
}

static constexpr UInt32 s_ExtraHotkeyVtbl = 0xDB58D4;
static constexpr UInt32 s_ExtraOwnershipVtbl = 0xDB5850;
static constexpr UInt32 s_ExtraRankVtbl = 0xDB5868;
static constexpr UInt32 s_ExtraScriptVtbl = 0xDB58BC;

ExtraOwnership* ExtraOwnership::Create()
{
	auto xOwn = static_cast<ExtraOwnership*>(BSExtraData::Create(kExtraData_Ownership, sizeof(ExtraOwnership),
	                                                             s_ExtraOwnershipVtbl));
	xOwn->owner = nullptr;
	return xOwn;
}

ExtraRank* ExtraRank::Create()
{
	auto xRank = static_cast<ExtraRank*>(BSExtraData::Create(kExtraData_Rank, sizeof(ExtraRank), s_ExtraRankVtbl));
	xRank->rank = 0;
	return xRank;
}

ExtraScript* ExtraScript::Create(TESForm* baseForm, bool create, TESObjectREFR* container)
{
	auto xScript = static_cast<ExtraScript*>(BSExtraData::Create(kExtraData_Script, sizeof(ExtraScript), s_ExtraScriptVtbl));
	if (xScript && baseForm)
	{
		auto pScript = DYNAMIC_CAST(baseForm, TESForm, TESScriptableForm);
		if (pScript && pScript->script)
		{
			xScript->script = pScript->script;
			if (create)
			{
				xScript->eventList = xScript->script->CreateEventList();
				if (container) xScript->EventCreate(0x00000001, container);
			}
		}
	}
	return xScript;
}

void ExtraScript::EventCreate(UInt32 eventCode, TESObjectREFR* container)
{
	if (eventList)
	{
		// create Event struct
		auto pEvent = static_cast<ScriptEventList::Event*>(FormHeap_Allocate(sizeof(ScriptEventList::Event)));
		if (pEvent)
		{
			pEvent->eventMask = eventCode;
			pEvent->object = (TESForm*)container;
		}

		if (!eventList->m_eventList)
		{
			eventList->m_eventList = static_cast<ScriptEventList::EventList*>(FormHeap_Allocate(
				sizeof(ScriptEventList::EventList)));
			eventList->m_eventList->Init();
		}
		if (eventList->m_eventList && pEvent) eventList->m_eventList->AddAt(pEvent, 0);
	}
}

ExtraHotkey* ExtraHotkey::Create()
{
	auto xHotkey = static_cast<ExtraHotkey*>(BSExtraData::Create(kExtraData_Hotkey, sizeof(ExtraHotkey), s_ExtraHotkeyVtbl));
	xHotkey->index = 0;
	return xHotkey;
}
