#pragma once

#include <list>
#include "Utilities.h"




// 8
class BSString
{
public:
	BSString();
	~BSString();

	char	* m_data;
	UInt16	m_dataLen;
	UInt16	m_bufLen;

	void Init(UInt32 bufSize);
	bool Set(const char* src);
	bool Append(const char* toAppend);
	void AppendChar(char toAppend);
	void InsertChar(char toInsert, UInt32 index);
	void EraseAt(UInt32 index);
	const char* CStr();
};

enum {
	eListCount = -3,
	eListEnd = -2,
	eListInvalid = -1,		
};
template <typename T_Data> struct ListNode
{
	T_Data* data;
	ListNode* next;

	ListNode() : data(NULL), next(NULL) {}
	ListNode(T_Data* _data) : data(_data), next(NULL) {}

	T_Data* Data() const { return data; }
	ListNode* Next() const { return next; }

	ListNode* RemoveMe()
	{
		if (next)
		{
			ListNode* pNext = next;
			data = next->data;
			next = next->next;
			GameHeapFree(pNext);
			return this;
		}
		data = NULL;
		return NULL;
	}

	ListNode* RemoveNext()
	{
		ListNode* pNext = next;
		next = next->next;
		GameHeapFree(pNext);
		return next;
	}

	ListNode* Append(T_Data* _data)
	{
		ListNode* newNode = (ListNode*)GameHeapAlloc(sizeof(ListNode));
		newNode->data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}

	ListNode* Insert(T_Data* _data)
	{
		ListNode* newNode = (ListNode*)GameHeapAlloc(sizeof(ListNode));
		newNode->data = data;
		data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}
};

UInt32 GetRandomIntBelow(int max);
template <class Item> class tList
{
public:
	typedef ListNode<Item> Node;

private:
	Node	m_listHead;

	template <class Op>
	UInt32 FreeNodes(Node* node, Op& compareOp) const
	{
		static UInt32 nodeCount = 0, numFreed = 0, lastNumFreed = 0;
		if (node->next)
		{
			nodeCount++;
			FreeNodes(node->next, compareOp);
			nodeCount--;
		}
		if (compareOp.Accept(node->data))
		{
			node->RemoveMe();
			numFreed++;
		}
		if (!nodeCount)
		{
			lastNumFreed = numFreed;
			numFreed = 0;
		}
		return lastNumFreed;
	}

public:
	void Init(Item* item = NULL)
	{
		m_listHead.data = item;
		m_listHead.next = NULL;
	}

	Node* GetLastNode(SInt32* outIdx = NULL) const
	{
		SInt32 index = 0;
		Node* node = Head();
		while (node->next)
		{
			node = node->next;
			index++;
		}
		if (outIdx) *outIdx = index;
		return node;
	}

	Node* GetNthNode(SInt32 index) const
	{
		if (index >= 0)
		{
			Node* node = Head();
			do
			{
				if (!index) return node;
				index--;
			} while (node = node->next);
		}
		return NULL;
	}

	Node* Head() const { return const_cast<Node*>(&m_listHead); }

	bool Empty() const { return !m_listHead.data; }

	class Iterator
	{
		Node* m_curr;

	public:
		Iterator operator++()
		{
			if (m_curr) m_curr = m_curr->next;
			return *this;
		}
		bool End() const { return !m_curr || (!m_curr->data && !m_curr->next); }
		Item* operator->() const { return m_curr->data; }
		Item*& operator*() const { return m_curr->data; }
		const Iterator& operator=(const Iterator& rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}
		bool operator==(const Iterator& other) const
		{
			return m_curr == other.m_curr;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_curr != other.m_curr;
		}

		Item* Get() const { return m_curr->data; }
		void Next() { if (m_curr) m_curr = m_curr->next; }
		void Find(Item* _item)
		{
			while (m_curr)
			{
				if (m_curr->data == _item) break;
				m_curr = m_curr->next;
			}
		}

		Iterator(Node* node = NULL) : m_curr(node) {}
		Iterator(tList& _list) : m_curr(&_list.m_listHead) {}
		Iterator(tList* _list) : m_curr(&_list->m_listHead) {}
		Iterator(tList& _list, Item* _item) : m_curr(&_list.m_listHead) { Find(_item); }
		Iterator(tList* _list, Item* _item) : m_curr(&_list->m_listHead) { Find(_item); }
	};

	const Iterator Begin() const { return Iterator(Head()); }
	const Iterator begin() const { return Iterator(Head()); }
	const Iterator end() const { return Iterator(nullptr); }

	UInt32 Count() const
	{
		if (!m_listHead.data) return 0;
		Node* node = Head();
		UInt32 count = 1;
		while (node = node->next) count++;
		return count;
	};

	bool Contains(Item* item) const
	{
		Node* node = Head();
		do
		{
			if (node->data == item) return true;
			node = node->next;
		} while (node);
		return false;
	}

	Item* GetFirstItem() const
	{
		return m_listHead.data;
	}

	Item* GetLastItem() const
	{
		return GetLastNode()->data;
	}

	Item* GetNthItem(SInt32 index) const
	{
		if (eListEnd == index)
			return GetLastNode()->data;
		Node* node = GetNthNode(index);
		return node ? node->data : NULL;
	}

	Item* GetRandomItem() const
	{
		int numItems = Count();
		int itemIdx = GetRandomIntBelow(numItems);
		return GetNthItem(itemIdx);
	}

	SInt32 AddAt(Item* item, SInt32 index)
	{
		if (!item) return eListInvalid;
		Node* node;
		if (!index)
		{
			if (m_listHead.data) m_listHead.Insert(item);
			else m_listHead.data = item;
		}
		else if (eListEnd == index)
		{
			node = GetLastNode(&index);
			if (node->data) node->Append(item);
			else node->data = item;
		}
		else
		{
			node = GetNthNode(index);
			if (!node) return eListInvalid;
			node->Insert(item);
		}
		return index;
	}

	SInt32 Append(Item* item)
	{
		SInt32 index = eListInvalid;
		if (item)
		{
			Node* node = GetLastNode(&index);
			if (node->data) node->Append(item);
			else node->data = item;
		}
		return index;
	}

	void Insert(Item* item)
	{
		if (item)
		{
			if (m_listHead.data) m_listHead.Insert(item);
			else m_listHead.data = item;
		}
	}

	void InsertSorted(Item* item, bool (*Compare)(Item* a, Item* b))
	{
		Node* curr = Head();
		while (curr)
		{
			if (!curr->data || !Compare(item, curr->data)) break;
			curr = curr->next;
		}
		curr->Insert(item);
	}

	void CopyFrom(tList& sourceList)
	{
		Node* target = Head(), * source = sourceList.Head();
		RemoveAll();
		if (!source->data) return;
		target->data = source->data;
		while (source = source->next)
			target = target->Append(source->data);
	}

	template <class Op>
	void Visit(Op& op, Node* prev = NULL) const
	{
		Node* curr = prev ? prev->next : Head();
		while (curr)
		{
			if (!curr->data || !op.Accept(curr->data)) break;
			curr = curr->next;
		}
	}

	template <class Op>
	Item* Find(Op& op) const
	{
		Node* curr = Head();
		Item* pItem;
		do
		{
			pItem = curr->data;
			if (pItem && op.Accept(pItem)) return pItem;
			curr = curr->next;
		} while (curr);
		return NULL;
	}

	template <class Op>
	Iterator Find(Op& op, Iterator& prev) const
	{
		Iterator curIt = prev.End() ? Begin() : ++prev;
		while (!curIt.End())
		{
			if (*curIt && op.Accept(*curIt)) break;
			++curIt;
		}
		return curIt;
	}



	template <class Op>
	UInt32 CountIf(Op& op) const
	{
		UInt32 count = 0;
		Node* curr = Head();
		do
		{
			if (curr->data && op.Accept(curr->data)) count++;
			curr = curr->next;
		} while (curr);
		return count;
	}

	class AcceptAll
	{
	public:
		bool Accept(Item* item) { return true; }
	};

	void RemoveAll() const
	{
		Node* nextNode = Head(), * currNode = nextNode->next;
		nextNode->data = NULL;
		nextNode->next = NULL;
		while (currNode)
		{
			nextNode = currNode->next;
			GameHeapFree(currNode);
			currNode = nextNode;
		}
	}

	void DeleteAll() const
	{
		Node* nextNode = Head(), * currNode = nextNode->next;
		GameHeapFree(nextNode->data);
		nextNode->data = NULL;
		nextNode->next = NULL;
		while (currNode)
		{
			nextNode = currNode->next;
			GameHeapFree(currNode->data);
			GameHeapFree(currNode);
			currNode = nextNode;
		}
	}

	void DeleteHead() const
	{
		auto node = Head();
		if (node->data)
		{
			GameHeapFree(node->data);
			node->data = NULL;
		}

		if (auto next = node->next)
		{
			node->data = next->data;
			node->next = next->next;
			GameHeapFree(next);
		}
	}

	Item* RemoveNth(SInt32 idx)
	{
		Item* removed = NULL;
		if (idx <= 0)
		{
			removed = m_listHead.data;
			m_listHead.RemoveMe();
		}
		else
		{
			Node* node = Head();
			while (node->next && --idx)
				node = node->next;
			if (!idx)
			{
				removed = node->next->data;
				node->RemoveNext();
			}
		}
		return removed;
	};

	UInt32 Remove(Item* item)
	{
		UInt32 removed = 0;
		Node* curr = Head(), * prev = NULL;
		do
		{
			if (curr->data == item)
			{
				curr = prev ? prev->RemoveNext() : curr->RemoveMe();
				removed++;
			}
			else
			{
				prev = curr;
				curr = curr->next;
			}
		} while (curr);
		return removed;
	}

	void RemoveFirst(Item* item)
	{
		Node* curr = Head(), * prev = NULL;
		do
		{
			if (curr->data == item)
			{
				prev ? prev->RemoveNext() : curr->RemoveMe();
				return;
			}
			prev = curr;
		} while (curr = curr->next);
	}

	Item* ReplaceNth(SInt32 index, Item* item)
	{
		Item* replaced = NULL;
		if (item)
		{
			Node* node;
			if (eListEnd == index)
				node = GetLastNode();
			else
			{
				node = GetNthNode(index);
				if (!node) return NULL;
			}
			replaced = node->data;
			node->data = item;
		}
		return replaced;
	}

	UInt32 Replace(Item* item, Item* replace)
	{
		UInt32 replaced = 0;
		Node* curr = Head();
		do
		{
			if (curr->data == item)
			{
				curr->data = replace;
				replaced++;
			}
			curr = curr->next;
		} while (curr);
		return replaced;
	}

	template <class Op>
	UInt32 RemoveIf(Op& op)
	{
		return FreeNodes(Head(), op);
	}

	SInt32 GetIndexOf(Item* item)
	{
		SInt32 idx = 0;
		Node* curr = Head();
		do
		{
			if (curr->data == item) return idx;
			idx++;
			curr = curr->next;
		} while (curr);
		return -1;
	}

	template <class Op>
	SInt32 GetIndexOf(Op& op)
	{
		SInt32 idx = 0;
		Node* curr = Head();
		do
		{
			if (curr->data && op.Accept(curr->data)) return idx;
			idx++;
			curr = curr->next;
		} while (curr);
		return -1;
	}

	template <typename F>
	void ForEach(const F* f)
	{
		auto* node = Head();
		if (!node) return;
		do
		{
			f(node->data);
		} while (node = node->next)
	}
};
STATIC_ASSERT(sizeof(tList<void*>) == 0x8);

template <typename T_Data> struct DListNode
{
public:
	DListNode* next;
	DListNode* prev;
	T_Data* data;

	DListNode* Advance(UInt32 times)
	{
		DListNode* result = this;
		while (result && times)
		{
			times--;
			result = result->next;
		}
		return result;
	}

	DListNode* Regress(UInt32 times)
	{
		DListNode* result = this;
		while (result && times)
		{
			times--;
			result = result->prev;
		}
		return result;
	}

	T_Data* GetAndAdvance()
	{
		T_Data* item = nullptr;
		if (next)
		{
			item = next->data;
			next = next->next;
		}
		return item;
	}

	T_Data* GetAndRegress()
	{
		T_Data* item = nullptr;
		if (next)
		{
			item = next->data;
			next = next->prev;
		}
		return item;
	}
};

template <class Item> class DList
{
public:
	typedef DListNode<Item> Node;

	// Nested iterator class
	class Iterator {
		Node* current;
	public:
		Iterator(Node* node) : current(node) {}

		Iterator& operator++() {
			current = current->next;
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return current != other.current;
		}

		Item* operator*() const {
			return current->data;
		}
	};

private:
	Node* first;
	Node* last;
	UInt32		count;

public:
	bool Empty() const { return !first; }
	Node* Head() { return first; }
	Node* Tail() { return last; }
	UInt32 Size() const { return count; }
	void SetHead(Node* head) { first = head; };
	void Init() { first = nullptr; last = nullptr; count = 0; };
	void Append(Item* item)
	{
		ThisCall(0x6E9670, this, &item);
	}
	Node* Remove(Item* item)
	{
		// return the item before the removed entry, or first if previous is null
		Node* result = nullptr;

		Node* node = Head();
		if (!node) return result;

		if (node->data == item)
		{
			first = node->next;
			if (first)
			{
				first->prev = nullptr;
			}
			else
			{
				last = nullptr;
			}

			--count;
			ThisCall(0xADE560, this, node);
			node = first;
		}
		else
		{
			node = node->next;
		}

		if (node)
		{
			do
			{
				if (node->data == item)
				{
					result = node->prev;

					node->prev->next = node->next;
					if (node->next)
					{
						node->next->prev = node->prev;
					}
					else
					{
						last = node->prev;
					}
					--count;
					ThisCall(0xADE560, this, node);
				}
			} while (node = node->next);
		}

		return result ? result : first;
	}

	void Sort(int (*compare)(Item* a, Item* b))
	{
		if (!first) return;
		Node* current = first;
		while (current->next)
		{
			Node* index = current->next;
			while (index)
			{
				if (compare(current->data, index->data) > 0)
				{
					Item* temp = current->data;
					current->data = index->data;
					index->data = temp;
				}
				index = index->next;
			}
			current = current->next;
		}
	}

	// begin and end functions for iterator support
	Iterator begin() {
		return Iterator(first);
	}

	Iterator end() {
		return Iterator(nullptr); // end iterator points to nullptr
	}
};

// 010
template <class T>
class BSSimpleList
{
public:
	BSSimpleList<T>();
	~BSSimpleList<T>();
	
	void**		_vtbl;
	tList<T>	list;
};
STATIC_ASSERT(sizeof(BSSimpleList<void *>) == 0xC);

#if RUNTIME

	const UInt32 _NiTMap_Lookup = 0x00BEA7B0;

#else
	const UInt32 _NiTMap_Lookup = 0;
#endif


	template <typename T_Key, typename T_Data> struct NiTMapEntry
	{
		NiTMapEntry* next;
		T_Key			key;
		T_Data			data;
	};

	template <typename T_Key, typename T_Data>
	class NiTMapBase
	{
	public:
		NiTMapBase();
		~NiTMapBase();

		typedef NiTMapEntry<T_Key, T_Data> Entry;

		virtual NiTMapBase* Destructor(bool doFree);
		virtual UInt32		CalculateBucket(T_Key key);
		virtual bool		Equal(T_Key key1, T_Key key2);
		virtual void		FillEntry(Entry* entry, T_Key key, T_Data data);
		virtual	void		Unk_004(void* arg0);
		virtual	void		Unk_005();
		virtual	void		Unk_006();

		UInt32		numBuckets;	// 04
		Entry**		buckets;	// 08
		UInt32		numItems;	// 0C

		class Iterator
		{
			friend NiTMapBase;

			NiTMapBase* m_table;
			Entry* m_entry;
			UInt32			m_bucket;

			void FindValid()
			{
				for (; m_bucket < m_table->numBuckets; m_bucket++)
				{
					m_entry = m_table->buckets[m_bucket];
					if (m_entry) break;
				}
			}

		public:
			Iterator(NiTMapBase* table) : m_table(table), m_entry(NULL), m_bucket(0) { FindValid(); }

			bool Done() const { return m_entry == NULL; }
			void Next()
			{
				m_entry = m_entry->next;
				if (!m_entry)
				{
					m_bucket++;
					FindValid();
				}
			}
			T_Data Get() const { return m_entry->data; }
			T_Key Key() const { return m_entry->key; }
		};

		Entry* Get(T_Key key);
	};
	template <typename T>
	struct BSSimpleArray
	{
		void* _vtbl;		// 00
		T* data;			// 04
		UInt32	size;			// 08
		UInt32	alloc;			// 0C

		// this only compiles for pointer types
		T operator[](UInt32 idx) {
			if (idx < size)
				return data[idx];
			return NULL;
		}
	};
template <typename T_Data>
	struct NiTArray
	{
		virtual NiTArray* Destroy(UInt32 doFree);

		T_Data* data;			// 04
		UInt16		capacity;		// 08 - init'd to size of preallocation
		UInt16		firstFreeEntry;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
		UInt16		numObjs;		// 0C - init'd to 0
		UInt16		growSize;		// 0E - init'd to size of preallocation

		T_Data operator[](UInt32 idx)
		{
			if (idx < firstFreeEntry)
				return data[idx];
			return NULL;
		}

		T_Data Get(UInt32 idx) { return data[idx]; }

		UInt16 Length() { return firstFreeEntry; }
		void AddAtIndex(UInt32 index, T_Data* item);	// no bounds checking
		void SetCapacity(UInt16 newCapacity);	// grow and copy data if needed

		class Iterator
		{
			friend NiTArray;

			T_Data* pData;
			UInt32		count;

		public:
			explicit operator bool() const { return count != 0; }
			bool End() const { return !count; }
			void operator++()
			{
				pData++;
				count--;
			}

			T_Data& operator*() const { return *pData; }
			T_Data& operator->() const { return *pData; }
			T_Data& Get() const { return *pData; }

			Iterator(NiTArray& source) : pData(source.data), count(source.firstFreeEntry) {}
		};

		Iterator Begin() { return Iterator(*this); }
	};
// this is a NiTPointerMap <UInt32, T_Data>
// todo: generalize key
template <typename T_Data>
class NiTPointerMap
{
public:
	struct Entry
	{
		Entry* next;
		UInt32		key;
		T_Data* data;
	};

	NiTPointerMap();
	virtual ~NiTPointerMap();

	virtual UInt32	CalculateBucket(UInt32 key);
	virtual bool	CompareKey(UInt32 lhs, UInt32 rhs);
	virtual void	Fn_03(UInt32 arg0, UInt32 arg1, UInt32 arg2);	// assign to entry
	virtual void	Fn_04(UInt32 arg);
	virtual Entry* AllocNewEntry(void);	// locked operations
	virtual void	FreeEntry(Entry*);	// locked operations

	UInt32		m_numBuckets;	// 04
	Entry** m_buckets;	// 08
	UInt32		m_numItems;		// 0C

	T_Data* Lookup(UInt32 key) const
	{
		for (Entry* traverse = m_buckets[key % m_numBuckets]; traverse; traverse = traverse->next)
			if (traverse->key == key) return traverse->data;
		return NULL;
	}

	bool Insert(Entry* newEntry)
	{
		UInt32 bucket = newEntry->key % m_numBuckets;
		Entry* entry = m_buckets[bucket], * prev;
		if (entry)
		{
			do
			{
				if (entry->key == newEntry->key) return false;
				prev = entry;
			} while (entry = entry->next);
			prev->next = newEntry;
		}
		else m_buckets[bucket] = newEntry;
		m_numItems++;
		return true;
	}

	void Insert(UInt32 key, T_Data* val)
	{
		UInt32 bucket = key % m_numBuckets;
		Entry* entry = m_buckets[bucket];
		if (entry)
		{
			do
			{
				if (entry->key == key)
				{
					entry->data = val;
					return;
				}
			} while (entry = entry->next);
		}
		auto newEntry = AllocNewEntry();
		newEntry->next = m_buckets[bucket];
		newEntry->key = key;
		newEntry->data = val;
		m_buckets[bucket] = newEntry;
		m_numItems++;
		return;
	}

	bool Remove(UInt32 key)
	{
		UInt32 bucket = key % m_numBuckets;
		Entry* entry = m_buckets[bucket];
		if (!entry) return false;

		if (entry->key == key)
		{
			m_buckets[bucket] = entry->next;
			FreeEntry(entry);
			--m_numItems;
			return true;
		}

		auto prevEntry = entry;
		auto iter = entry->next;
		for (; iter && iter->key != key; iter = iter->next)
		{
			prevEntry = iter;
		}
		if (!iter)
		{
			return false;
		}
		prevEntry->next = iter->next;
		FreeEntry(iter);
		--m_numItems;
		return true;
	}

	// assume CalculateBucket just takes modulo (true for int maps)
	Entry* GetNextFreeEntry(Entry* entry)
	{
		if (entry->next) return entry->next;
		for (UInt32 i = (entry->key % m_numBuckets) + 1; i < m_numBuckets; ++i)
		{
			if (m_buckets[i])
			{
				return m_buckets[i];
			}
		}
		return nullptr;
	}

	// use the vtable method for getting next bucket
	Entry* GetNextFreeEntryAlt(Entry* entry)
	{
		if (entry->next) return entry->next;
		for (UInt32 i = this->CalculateBucket(entry->key) + 1; i < m_numBuckets; ++i)
		{
			if (m_buckets[i])
			{
				return m_buckets[i];
			}
		}
		return nullptr;
	}

	void Dump()
	{
		for (int i = 0; i < m_numBuckets; ++i)
		{
			Entry* iter = m_buckets[i];
			int count = 0;
			if (iter)
			{
				do
				{
					++count;
				} while (iter = iter->next);
			}
			Console_Print("Bucket %d: %d", i, count);
		}
	}

	class Iterator
	{
		friend NiTPointerMap;

		NiTPointerMap* m_table;
		Entry* m_entry;
		UInt32			m_bucket;

		void FindValid()
		{
			for (; m_bucket < m_table->m_numBuckets; m_bucket++)
			{
				m_entry = m_table->m_buckets[m_bucket];
				if (m_entry) break;
			}
		}

	public:
		Iterator(NiTPointerMap* table) : m_table(table), m_entry(NULL), m_bucket(0) { FindValid(); }

		bool Done() const { return m_entry == NULL; }
		void Next()
		{
			m_entry = m_entry->next;
			if (!m_entry)
			{
				m_bucket++;
				FindValid();
			}
		}
		T_Data* Get() const { return m_entry->data; }
		UInt32 Key() const { return m_entry->key; }
	};

	const Iterator Begin() const { return Iterator((NiTPointerMap<T_Data>*)this); }
};

template <typename T_Key, typename T_Data>
__declspec(naked) NiTMapEntry<T_Key, T_Data>* NiTMapBase<T_Key, T_Data>::Get(T_Key key)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		eax, [esp + 0xC]
		push	eax
		mov		eax, [ecx]
		call	dword ptr[eax + 4]
		mov		ecx, [esi + 8]
		mov		edi, [ecx + eax * 4]
		findEntry:
		test	edi, edi
			jz		done
			mov		eax, [esp + 0xC]
			push	dword ptr[edi + 4]
			push	eax
			mov		ecx, esi
			mov		eax, [ecx]
			call	dword ptr[eax + 8]
			test	al, al
			jnz		done
			mov		edi, [edi]
			jmp		findEntry
			done :
		mov		eax, edi
			pop		edi
			pop		esi
			retn	4
	}
}

// 10
struct SystemColorManager {
	// 0C
	class SystemColor {
	public:
		SystemColor();
		~SystemColor();

		virtual void	Destructor(bool doFree);
		virtual UInt32	GetColor();
		virtual void	SetColor(UInt32 newColor);
		virtual bool	IsHard();
		virtual bool	IsSoft();

		BSString		traitName;

		void SetColorRGB(UInt32 r, UInt32 g, UInt32 b) { this->SetColor(((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF)); }
	};

	// 10
	class HardSystemColor : public SystemColor {
	public:
		HardSystemColor();
		~HardSystemColor();

		UInt32		color;		// 0C
	};

	// 10
	class SoftSystemColor : public SystemColor {
	public:
		SoftSystemColor();
		~SoftSystemColor();

		UInt32* setting;	// 0C
	};

	DList<SystemColor>	sysColors;
	UInt32				unk0C;

	static SystemColorManager* GetSingleton() { return StdCall<SystemColorManager*>(0x6295A0); }
	UInt32 GetColor(UInt32 type) { return ThisCall<UInt32>(0x621640, this, type); }
};
