#include "GameExtraData.h"
#include "GameBSExtraData.h"
#include "GameRTTI.h"


struct GetMatchingEquipped {	
	FormMatcher& m_matcher;
	EquipData m_found;

	GetMatchingEquipped(FormMatcher& matcher) : m_matcher(matcher) {
		m_found.pForm = NULL;
		m_found.pExtraData = NULL;
	}

	bool Accept(ExtraContainerChanges::EntryData* pEntryData) {
		if (pEntryData) {
			// quick check - needs an extendData or can't be equipped
			ExtraContainerChanges::ExtendDataList* pExtendList = pEntryData->extendData;
			if (pExtendList && m_matcher.Matches(pEntryData->type)) { 
				SInt32 n = 0;
				ExtraDataList* pExtraDataList = pExtendList->GetNthItem(n);
				while (pExtraDataList) {
					if (pExtraDataList->HasType(kExtraData_Worn) || pExtraDataList->HasType(kExtraData_WornLeft)) {
						m_found.pForm = pEntryData->type;
						m_found.pExtraData = pExtraDataList;
						return false;
					}
					n++;
					pExtraDataList = pExtendList->GetNthItem(n);
				}
			}
		}
		return true;
	}

	EquipData Found() {
		return m_found;
	}
};


EquipData ExtraContainerChanges::FindEquipped(FormMatcher& matcher) const
{
	FoundEquipData equipData;
	if (data && data->objList) {
		GetMatchingEquipped getEquipped(matcher);
		data->objList->Visit(getEquipped);
		equipData = getEquipped.Found();
	}
	return equipData;
};


static const UInt32 s_ExtraHealthVtbl = 0x00DB5880;
static const UInt32 s_ExtraLockVtbl = 0x00DB582C;
static const UInt32 s_ExtraCountVtbl = 0x00DB5874;
static const UInt32 s_ExtraOwnershipVtbl = 0xDB5850;
static const UInt32 s_ExtraRankVtbl = 0xDB5868;
static const UInt32 s_ExtraScriptVtbl = 0xDB58BC;
static const UInt32 s_ExtraHotkeyVtbl = 0xDB58D4;

STATIC_ASSERT(sizeof(ExtraHealth) == 0x10);
STATIC_ASSERT(sizeof(ExtraLock) == 0x10);
STATIC_ASSERT(sizeof(ExtraCount) == 0x10);

ExtraContainerChanges::ExtendDataList* ExtraContainerChangesExtendDataListCreate(ExtraDataList* pExtraDataList)
{
	ExtraContainerChanges::ExtendDataList* xData = (ExtraContainerChanges::ExtendDataList*)GameHeapAlloc(sizeof(ExtraContainerChanges::ExtendDataList));
	xData->Init();
	if (pExtraDataList) xData->Append(pExtraDataList);
	return xData;
}

void ExtraContainerChanges::EntryData::Cleanup()
{
	if (!extendData) return;
	ListNode<ExtraDataList>* xdlIter = extendData->Head(), * prev = NULL;;
	ExtraDataList* xData;
	ExtraCount* xCount;
	do
	{
		xData = xdlIter->data;
		if (xData)
		{
			xCount = (ExtraCount*)xData->GetByType(kExtraData_Count);
			if (xCount && (xCount->count <= 1)) xData->RemoveByType(kExtraData_Count);
			if (xData->m_data)
			{
				prev = xdlIter;
				xdlIter = xdlIter->next;
				continue;
			}
		}
		GameHeapFree(xData);
		xdlIter = prev ? prev->RemoveNext() : xdlIter->RemoveMe();
	} while (xdlIter);
}

void ExtraContainerChanges::EntryData::Destroy()
{
	ThisCall(0x47FD30, this);
	FormHeap_Free(this);
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

ExtraContainerChanges::EntryData* ExtraContainerChanges::Data::GetEntryDataEquippedItem(UInt32 bodyPart, bool a2)
{
	return ThisCall<ExtraContainerChanges::EntryData*>(0x483EB0, this, bodyPart, a2);
}

void ExtraContainerChanges::Cleanup()
{
	if (data && data->objList)
	{
		ListNode<EntryData>* entryIter = data->objList->Head();
		do
		{
			if (entryIter->data)
				entryIter->data->Cleanup();
		} while (entryIter = entryIter->next);
	}
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

ExtraHealth* ExtraHealth::Create() 
{
	ExtraHealth* xHealth = (ExtraHealth*)BSExtraData::Create(kExtraData_Health, sizeof(ExtraHealth), s_ExtraHealthVtbl);
	return xHealth;
}

ExtraLock* ExtraLock::Create()
{
	ExtraLock* xLock = (ExtraLock*)BSExtraData::Create(kExtraData_Lock, sizeof(ExtraLock), s_ExtraLockVtbl);
	ExtraLock::Data* lockData = (ExtraLock::Data*)FormHeap_Allocate(sizeof(ExtraLock::Data));
	memset(lockData, 0, sizeof(ExtraLock::Data));
	xLock->data = lockData;
	return xLock;
}

ExtraCount* ExtraCount::Create()
{
	ExtraCount* xCount = (ExtraCount*)BSExtraData::Create(kExtraData_Count, sizeof(ExtraCount), s_ExtraCountVtbl);
	xCount->count = 0;
	return xCount;
}

ExtraOwnership* ExtraOwnership::Create()
{
	ExtraOwnership* xOwn = (ExtraOwnership*)BSExtraData::Create(kExtraData_Ownership, sizeof(ExtraOwnership), s_ExtraOwnershipVtbl);
	xOwn->owner = nullptr;
	return xOwn;
}

ExtraRank* ExtraRank::Create()
{
	ExtraRank* xRank = (ExtraRank*)BSExtraData::Create(kExtraData_Rank, sizeof(ExtraRank), s_ExtraRankVtbl);
	xRank->rank = 0;
	return xRank;
}

ExtraScript* ExtraScript::Create(TESForm* baseForm, bool create, TESObjectREFR* container)
{
	ExtraScript* xScript = (ExtraScript*)BSExtraData::Create(kExtraData_Script, sizeof(ExtraScript), s_ExtraScriptVtbl);
	if (xScript && baseForm) {
		TESScriptableForm* pScript = DYNAMIC_CAST(baseForm, TESForm, TESScriptableForm);
		if (pScript && pScript->script) {
			xScript->script = pScript->script;
			if (create) {
				xScript->eventList = xScript->script->CreateEventList();
				if (container)
					xScript->EventCreate(0x00000001, container);
			}
		}
	}
	return xScript;
}

void ExtraScript::EventCreate(UInt32 eventCode, TESObjectREFR* container)
{
	if (eventList) {
		// create Event struct
		ScriptEventList::Event* pEvent = (ScriptEventList::Event*)FormHeap_Allocate(sizeof(ScriptEventList::Event));
		if (pEvent) {
			pEvent->eventMask = eventCode;
			pEvent->object = (TESForm*)container;
		}

		if (!eventList->m_eventList) {
			eventList->m_eventList = (ScriptEventList::EventList*)FormHeap_Allocate(sizeof(ScriptEventList::EventList));
			eventList->m_eventList->Init();
		}
		if (eventList->m_eventList && pEvent)
			eventList->m_eventList->AddAt(pEvent, 0);
	}
}

ExtraHotkey* ExtraHotkey::Create()
{
	ExtraHotkey* xHotkey = (ExtraHotkey*)BSExtraData::Create(kExtraData_Hotkey, sizeof(ExtraHotkey), s_ExtraHotkeyVtbl);
	xHotkey->index = 0;
	return xHotkey;
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
