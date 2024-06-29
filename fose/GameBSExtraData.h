#pragma once

// Added to remove a cyclic dependency between GameForms.h and GameExtraData.h

// C+?
class BSExtraData
{
public:
	BSExtraData();
	virtual ~BSExtraData();

	virtual void	Fn_01(void);

	static BSExtraData* Create(UInt8 xType, UInt32 size, UInt32 vtbl);

//	void		** _vtbl;	// 000
	UInt8		type;		// 004
	UInt8		pad[3];		// 005
	BSExtraData	* next;		// 008
};

struct BaseExtraList
{
	virtual void Destroy(bool doFree);

	BSExtraData* m_data;					// 004
	UInt8			m_presenceBitfield[18];	// 008 - if a bit is set, then the extralist should contain that extradata
	UInt8			pad[2];
	bool HasType(UInt32 type) const;
	BSExtraData* GetByType(UInt32 type) const;
	void MarkType(UInt32 type, bool bCleared);
	void Remove(BSExtraData* toRemove, bool doFree = false);
	void RemoveByType(UInt32 type);
	BSExtraData* Add(BSExtraData* xData);
	void RemoveAll(bool doFree = true);
	void Copy(BaseExtraList* sourceList);
};

struct ExtraDataList : public BaseExtraList
{
	ExtraDataList* CreateCopy();
	static ExtraDataList* __stdcall Create(BSExtraData* xBSData = NULL);
};

STATIC_ASSERT(sizeof(ExtraDataList) == 0x01C);


