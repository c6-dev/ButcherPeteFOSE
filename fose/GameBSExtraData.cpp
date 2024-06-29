#include "GameBSExtraData.h"
#include "GameAPI.h"



bool BaseExtraList::HasType(UInt32 type) const
{
	UInt32 index = (type >> 3);
	UInt8 bitMask = 1 << (type % 8);
	return (m_presenceBitfield[index] & bitMask) != 0;
}

[[nodiscard]] BSExtraData* BaseExtraList::GetByType(UInt32 type) const
{
	return ThisCall<BSExtraData*>(0x40A4C0, this, type);
}

void BaseExtraList::MarkType(UInt32 type, bool bCleared)
{
	UInt32 index = (type >> 3);
	UInt8 bitMask = 1 << (type % 8);
	UInt8& flag = m_presenceBitfield[index];
	if (bCleared) flag &= ~bitMask;
	else flag |= bitMask;
}

void BaseExtraList::Remove(BSExtraData* toRemove, bool doFree)
{
	ThisCall(0x40ECB0, this, toRemove, doFree);
}

BSExtraData* BaseExtraList::Add(BSExtraData* xData)
{
	return ThisCall<BSExtraData*>(0x40A320, this, xData);
}

ExtraDataList* ExtraDataList::Create(BSExtraData* xBSData)
{
	ExtraDataList* xData = (ExtraDataList*)GameHeapAlloc(sizeof(ExtraDataList));
	memset(xData, 0, sizeof(ExtraDataList));
	*(UInt32*)xData = 0xDB5C68;
	if (xBSData) xData->Add(xBSData);
	return xData;
}

void BaseExtraList::RemoveByType(UInt32 type)
{
	ThisCall(0x40A3F0, this, type);
}

void BaseExtraList::RemoveAll(bool doFree)
{
	ThisCall(0x40A6B0, this, doFree);
}

void BaseExtraList::Copy(BaseExtraList* sourceList)
{
	ThisCall(0x41B260, this, sourceList);
}


