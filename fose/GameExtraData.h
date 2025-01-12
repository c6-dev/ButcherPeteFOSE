#include "Utilities.h"
#include "GameBSExtraData.h"
#include "GameForms.h"

enum {
	kExtraData_Havok                    	= 0x01,
	kExtraData_Cell3D                   	= 0x02,
	kExtraData_CellWaterType            	= 0x03,
	kExtraData_RegionList               	= 0x04,
	kExtraData_SeenData                 	= 0x05,
	kExtraData_CellMusicType            	= 0x07,
	kExtraData_CellClimate              	= 0x08,
	kExtraData_ProcessMiddleLow         	= 0x09,
	kExtraData_CellCanopyShadowMask     	= 0x0A,
	kExtraData_DetachTime               	= 0x0B,
	kExtraData_PersistentCell           	= 0x0C,
	kExtraData_Script                   	= 0x0D,
	kExtraData_Action                   	= 0x0E,
	kExtraData_StartingPosition         	= 0x0F,
	kExtraData_Anim                     	= 0x10,
	kExtraData_UsedMarkers              	= 0x12,
	kExtraData_DistantData              	= 0x13,
	kExtraData_RagdollData              	= 0x14,
	kExtraData_ContainerChanges         	= 0x15,
	kExtraData_Worn                     	= 0x16,
	kExtraData_WornLeft                 	= 0x17,
	kExtraData_PackageStartLocation     	= 0x18,
	kExtraData_Package                  	= 0x19,
	kExtraData_TrespassPackage          	= 0x1A,
	kExtraData_RunOncePacks             	= 0x1B,
	kExtraData_ReferencePointer         	= 0x1C,
	kExtraData_Follower                 	= 0x1D,
	kExtraData_LevCreaModifier          	= 0x1E,
	kExtraData_Ghost                    	= 0x1F,
	kExtraData_OriginalReference        	= 0x20,
	kExtraData_Ownership                	= 0x21,
	kExtraData_Global                   	= 0x22,
	kExtraData_Rank                     	= 0x23,
	kExtraData_Count                    	= 0x24,
	kExtraData_Health                   	= 0x25,
	kExtraData_Uses                     	= 0x26,
	kExtraData_TimeLeft                 	= 0x27,
	kExtraData_Charge                   	= 0x28,
	kExtraData_Light                    	= 0x29,
	kExtraData_Lock                     	= 0x2A,
	kExtraData_Teleport                 	= 0x2B,
	kExtraData_MapMarker                	= 0x2C,
	kExtraData_LeveledCreature          	= 0x2E,
	kExtraData_LeveledItem              	= 0x2F,
	kExtraData_Scale                    	= 0x30,
	kExtraData_Seed                     	= 0x31,
	kExtraData_PlayerCrimeList          	= 0x35,
	kExtraData_EnableStateParent        	= 0x37,
	kExtraData_EnableStateChildren      	= 0x38,
	kExtraData_ItemDropper              	= 0x39,
	kExtraData_DroppedItemList          	= 0x3A,
	kExtraData_RandomTeleportMarker     	= 0x3B,
	kExtraData_MerchantContainer        	= 0x3C,
	kExtraData_SavedHavokData           	= 0x3D,
	kExtraData_CannotWear               	= 0x3E,
	kExtraData_Poison                   	= 0x3F,
	kExtraData_LastFinishedSequence     	= 0x41,
	kExtraData_SavedAnimation           	= 0x42,
	kExtraData_NorthRotation            	= 0x43,
	kExtraData_XTarget                  	= 0x44,
	kExtraData_FriendHits               	= 0x45,
	kExtraData_HeadingTarget            	= 0x46,
	kExtraData_RefractionProperty       	= 0x48,
	kExtraData_StartingWorldOrCell      	= 0x49,
	kExtraData_Hotkey                   	= 0x4A,
	kExtraData_EditorRefMovedData       	= 0x4C,
	kExtraData_InfoGeneralTopic         	= 0x4D,
	kExtraData_HasNoRumors              	= 0x4E,
	kExtraData_Sound                    	= 0x4F,
	kExtraData_TerminalState            	= 0x50,
	kExtraData_LinkedRef                	= 0x51,
	kExtraData_LinkedRefChildren        	= 0x52,
	kExtraData_ActivateRef              	= 0x53,
	kExtraData_ActivateRefChildren      	= 0x54,
	kExtraData_TalkingActor             	= 0x55,
	kExtraData_ObjectHealth             	= 0x56,
	kExtraData_DecalRefs                	= 0x57,
	kExtraData_CellImageSpace           	= 0x59,
	kExtraData_NavMeshPortal            	= 0x5A,
	kExtraData_ModelSwap                	= 0x5B,
	kExtraData_Radius                   	= 0x5C,
	kExtraData_Radiation                	= 0x5D,
	kExtraData_FactionChanges           	= 0x5E,
	kExtraData_DismemberedLimbs         	= 0x5F,
	kExtraData_MultiBound               	= 0x61,
	kExtraData_MultiBoundData           	= 0x62,
	kExtraData_MultiBoundRef            	= 0x63,
	kExtraData_ReflectedRefs            	= 0x65,
	kExtraData_ReflectorRefs            	= 0x66,
	kExtraData_EmittanceSource          	= 0x67,
	kExtraData_RadioData                	= 0x68,
	kExtraData_CombatStyle              	= 0x69,
	kExtraData_Primitive                	= 0x6B,
	kExtraData_OpenCloseActivateRef     	= 0x6C,
	kExtraData_Ammo                     	= 0x6E,
	kExtraData_PatrolRefData            	= 0x6F,
	kExtraData_PackageData              	= 0x70,
	kExtraData_OcclusionPlane           	= 0x71,
	kExtraData_CollisionData            	= 0x72,
	kExtraData_SayTopicInfoOnceADay     	= 0x73,
	kExtraData_EncounterZone            	= 0x74,
	kExtraData_SayToTopicInfo           	= 0x75,
	kExtraData_OcclusionPlaneRefData    	= 0x76,
	kExtraData_PortalRefData            	= 0x77,
	kExtraData_Portal                   	= 0x78,
	kExtraData_Room                     	= 0x79,
	kExtraData_HealthPerc               	= 0x7A,
	kExtraData_RoomRefData              	= 0x7B,
	kExtraData_GuardedRefData           	= 0x7C,
	kExtraData_CreatureAwakeSound       	= 0x7D,
	kExtraData_WaterZoneMap             	= 0x7E,
	kExtraData_IgnoredBySandbox         	= 0x80,
	kExtraData_CellAcousticSpace        	= 0x81,
	kExtraData_ReservedMarkers          	= 0x82,
	kExtraData_WeaponIdleSound          	= 0x83,
	kExtraData_WaterLightRefs           	= 0x84,
	kExtraData_LitWaterRefs             	= 0x85,
	kExtraData_WeaponAttackSound        	= 0x86,
	kExtraData_ActivateLoopSound        	= 0x87,
	kExtraData_PatrolRefInUseData       	= 0x88,
	kExtraData_AshPileRef               	= 0x89,
	kExtraData_CreatureMovementSound    	= 0x8A,
	kExtraData_FollowerSwimBreadcrumbs  	= 0x8B,
};

#define GetByTypeCast(xDataList, Type) DYNAMIC_CAST(xDataList.GetByType(kExtraData_ ## Type), BSExtraData, Extra ## Type)
#define GetExtraType(xDataList, Type) (Extra ## Type*)(xDataList).GetByType(kExtraData_ ## Type)

// 014
class ExtraHavok : public BSExtraData
{
public:
	ExtraHavok();
	virtual ~ExtraHavok();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraCell3D : public BSExtraData
{
	ExtraCell3D();
	virtual ~ExtraCell3D();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraCellWaterType : public BSExtraData
{
	ExtraCellWaterType();
	virtual ~ExtraCellWaterType();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraRegionList : public BSExtraData
{
	ExtraRegionList();
	virtual ~ExtraRegionList();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraSeenData : public BSExtraData
{
	ExtraSeenData();
	virtual ~ExtraSeenData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraCellMusicType : public BSExtraData
{
	ExtraCellMusicType();
	virtual ~ExtraCellMusicType();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraCellClimate : public BSExtraData
{
	ExtraCellClimate();
	virtual ~ExtraCellClimate();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraProcessMiddleLow : public BSExtraData
{
	ExtraProcessMiddleLow();
	virtual ~ExtraProcessMiddleLow();

	UInt32	unk00C;		// 00C
};

// 01C
class ExtraCellCanopyShadowMask : public BSExtraData
{
	ExtraCellCanopyShadowMask();
	virtual ~ExtraCellCanopyShadowMask();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
};

// 010
class ExtraDetachTime : public BSExtraData
{
	ExtraDetachTime();
	virtual ~ExtraDetachTime();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraPersistentCell : public BSExtraData
{
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraScript : public BSExtraData
{
public:
	ExtraScript();
	virtual ~ExtraScript();

	Script			* script;		// 00C
	ScriptEventList	* eventList;	// 010
	static ExtraScript* Create(TESForm* baseForm = NULL, bool create = true, TESObjectREFR* container = NULL);
	void EventCreate(UInt32 eventCode, TESObjectREFR* container);
};

// 014
class ExtraAction : public BSExtraData
{
	ExtraAction();
	virtual ~ExtraAction();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 024
class ExtraStartingPosition : public BSExtraData
{
	ExtraStartingPosition();
	virtual ~ExtraStartingPosition();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
	UInt32	unk020;		// 020
};

// 010
class ExtraAnim : public BSExtraData
{
	ExtraAnim();
	virtual ~ExtraAnim();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraUsedMarkers : public BSExtraData
{
	ExtraUsedMarkers();
	virtual ~ExtraUsedMarkers();

	UInt32	unk00C;		// 00C
};

// 018
class ExtraDistantData : public BSExtraData
{
	ExtraDistantData();
	virtual ~ExtraDistantData();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 010
class ExtraRagdollData : public BSExtraData
{
	ExtraRagdollData();
	virtual ~ExtraRagdollData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraContainerChanges : public BSExtraData
{
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();

	class ExtendDataList : public tList<ExtraDataList>
	{
	public:
		void Clear();
		ExtraDataList* RemoveExtra(ExtraDataList* xDataList, BSExtraData* xData);
		ExtraDataList* RemoveByType(ExtraDataList* xDataList, UInt32 type);
		void CleanEmpty();
	};

	struct EntryData
	{
		ExtendDataList* extendData;
		SInt32			countDelta;
		TESForm* type;

		EntryData(ListNode<ExtraDataList>* extend, SInt32 count, TESForm* item) :
			extendData((ExtendDataList*)extend), countDelta(count), type(item) {}

		void Cleanup();
		void Destroy();
		void RemoveCannotWear();
		float GetItemHealthPerc(bool arg1 = true);
		ExtraDataList* GetEquippedExtra();
		float CalculateWeaponDamage(float condition, TESForm* ammo);
		float GetValue();
		static EntryData* Create(TESForm* item, SInt32 count = 1);
		EntryData* Copy(EntryData* from);
		float GetWeight();
		ExtendDataList* Add(ExtraDataList* newList);
	};

	struct EntryDataList : tList<EntryData>
	{
		EntryData* FindForItem(TESForm* item);
		ExtraDataList* FindHighestConditionListForItem(TESForm* item);
		ExtraDataList* FindWorstItem(TESForm* item);
	};

	struct Data
	{
		tList<EntryData>* objList;
		TESObjectREFR* owner;
		float			totalWgCurrent;
		float			totalWgLast;
		UInt8			byte10;	// referenced in relation to scripts in container
		UInt8			pad[3];

		static Data* Create(TESObjectREFR* owner);
		float GetInventoryWeight();
		ExtraContainerChanges::EntryData* GetEntryDataEquippedItem(UInt32 bodyPart, bool a2 = false);
	};

	Data* data;	// 00C

	EntryData* GetByType(TESForm* type);

	void DebugDump();
	void Cleanup();	// clean up unneeded extra data from each EntryData

	static ExtraContainerChanges* Create();

	// find the equipped item whose form matches the passed matcher
	struct FoundEquipData {
		TESForm* pForm;
		ExtraDataList* pExtraData;
	};
	FoundEquipData FindEquipped(FormMatcher& matcher) const;
};
typedef ExtraContainerChanges::EntryData ContChangesEntry;
typedef ExtraContainerChanges::Data InventoryChanges;
typedef ExtraContainerChanges::FoundEquipData EquipData;
// 00C
class ExtraWorn : public BSExtraData
{
public:
	ExtraWorn();
	virtual ~ExtraWorn();
};

// 00C
class ExtraWornLeft : public BSExtraData
{
public:
	ExtraWornLeft();
	virtual ~ExtraWornLeft();
};

// 01C
class ExtraPackageStartLocation : public BSExtraData
{
	ExtraPackageStartLocation();
	virtual ~ExtraPackageStartLocation();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
};

// 01C
class ExtraPackage : public BSExtraData
{
	ExtraPackage();
	virtual ~ExtraPackage();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
};

// 010
class ExtraTrespassPackage : public BSExtraData
{
	ExtraTrespassPackage();
	virtual ~ExtraTrespassPackage();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraRunOncePacks : public BSExtraData
{
	ExtraRunOncePacks();
	virtual ~ExtraRunOncePacks();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraReferencePointer : public BSExtraData
{
	ExtraReferencePointer();
	virtual ~ExtraReferencePointer();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraFollower : public BSExtraData
{
	ExtraFollower();
	virtual ~ExtraFollower();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraLevCreaModifier : public BSExtraData
{
	ExtraLevCreaModifier();
	virtual ~ExtraLevCreaModifier();

	UInt32	unk00C;		// 00C
};

// 00C
class ExtraGhost : public BSExtraData
{
public:
	ExtraGhost();
	virtual ~ExtraGhost();
};

// 00C
class ExtraOriginalReference : public BSExtraData
{
public:
	ExtraOriginalReference();
	virtual ~ExtraOriginalReference();
};

// 010
class ExtraOwnership : public BSExtraData
{
public:
	ExtraOwnership();
	virtual ~ExtraOwnership();

	TESForm*	owner;	//maybe this should be a union {TESFaction*; TESNPC*} but it would be more unwieldy to access and modify
	static ExtraOwnership* Create();
};

// 010
class ExtraGlobal : public BSExtraData
{								//ownership data, stored separately from ExtraOwnership
public:
	ExtraGlobal();
	virtual ~ExtraGlobal();

	TESGlobal*	globalVar;	// 00C
};

// 010
class ExtraRank : public BSExtraData
{								//ownership data, stored separately from ExtraOwnership
public:
	ExtraRank();
	virtual ~ExtraRank();

	UInt32	rank; // 00C
	static ExtraRank* Create();
};

// 010
class ExtraCount : public BSExtraData
{
public:
	ExtraCount();
	virtual ~ExtraCount();

	SInt16	count;	// 00C
	UInt8	pad[2];	// 00E

	static ExtraCount* Create();	
};

// 010
class ExtraHealth : public BSExtraData
{
public:
	ExtraHealth();
	virtual ~ExtraHealth();
	float health;

	static ExtraHealth* Create();
};

// 010
class ExtraUses : public BSExtraData
{
public:
	ExtraUses();
	~ExtraUses();
	UInt32 unk0;
	static ExtraUses* Create();
};

// 010
class ExtraTimeLeft : public BSExtraData
{
public:
	ExtraTimeLeft();
	virtual ~ExtraTimeLeft();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraCharge : public BSExtraData
{
public:
	ExtraCharge();
	virtual ~ExtraCharge();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraLock : public BSExtraData
{
public:
	ExtraLock();
	virtual ~ExtraLock();

	struct Data
	{
		UInt32	lockLevel;
		TESKey	* key;
		UInt8	flags;
		UInt8	pad[3];
	};

	Data*	data;		// 00C
	static ExtraLock* Create();
};

// 010
class ExtraTeleport : public BSExtraData
{
public:
	ExtraTeleport();
	virtual ~ExtraTeleport();
	struct Data
	{
		TESObjectREFR*	linkedDoor;
		float			x; //x, y, z, zRot refer to teleport marker's position and rotation
		float			y; 
		float			z;
		float			zRot;
		UInt32			unk1;	//looks like flags (have only seen 0x80000000)
		UInt32			unk2;
	};

	Data *	data;	// 00C
};

// 010
class ExtraMapMarker : public BSExtraData
{
public:
	ExtraMapMarker();
	virtual ~ExtraMapMarker();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraLeveledCreature : public BSExtraData
{
public:
	ExtraLeveledCreature();
	virtual ~ExtraLeveledCreature();

	TESForm* baseForm;	// 00C
	TESForm* form;		// 010
};

// 014
class ExtraLeveledItem : public BSExtraData
{
	ExtraLeveledItem();
	virtual ~ExtraLeveledItem();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraScale : public BSExtraData
{
public:
	ExtraScale();
	virtual ~ExtraScale();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraSeed : public BSExtraData
{
public:
	ExtraSeed();
	virtual ~ExtraSeed();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraPlayerCrimeList : public BSExtraData
{
public:
	ExtraPlayerCrimeList();
	virtual ~ExtraPlayerCrimeList();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraEnableStateParent : public BSExtraData
{
	ExtraEnableStateParent();
	virtual ~ExtraEnableStateParent();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 014
class ExtraEnableStateChildren : public BSExtraData
{
	ExtraEnableStateChildren();
	virtual ~ExtraEnableStateChildren();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraItemDropper : public BSExtraData
{
public:
	ExtraItemDropper();
	virtual ~ExtraItemDropper();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraDroppedItemList : public BSExtraData
{
	ExtraDroppedItemList();
	virtual ~ExtraDroppedItemList();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraRandomTeleportMarker : public BSExtraData
{
public:
	ExtraRandomTeleportMarker();
	virtual ~ExtraRandomTeleportMarker();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraMerchantContainer : public BSExtraData
{
public:
	ExtraMerchantContainer();
	virtual ~ExtraMerchantContainer();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraSavedHavokData : public BSExtraData
{
public:
	ExtraSavedHavokData();
	virtual ~ExtraSavedHavokData();

	UInt32	unk00C;		// 00C
};

// 00C
class ExtraCannotWear : public BSExtraData
{
public:
	ExtraCannotWear();
	virtual ~ExtraCannotWear();
};

// 010
class ExtraPoison : public BSExtraData
{
public:
	ExtraPoison();
	virtual ~ExtraPoison();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraLastFinishedSequence : public BSExtraData
{
public:
	ExtraLastFinishedSequence();
	virtual ~ExtraLastFinishedSequence();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraSavedAnimation : public BSExtraData
{
public:
	ExtraSavedAnimation();
	virtual ~ExtraSavedAnimation();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraNorthRotation : public BSExtraData
{
public:
	ExtraNorthRotation();
	virtual ~ExtraNorthRotation();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraXTarget : public BSExtraData
{
public:
	ExtraXTarget();
	virtual ~ExtraXTarget();

	UInt32	unk00C;		// 00C
};

// 01C
class ExtraFriendHits : public BSExtraData
{
	ExtraFriendHits();
	virtual ~ExtraFriendHits();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
};

// 010
class ExtraHeadingTarget : public BSExtraData
{
public:
	ExtraHeadingTarget();
	virtual ~ExtraHeadingTarget();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraRefractionProperty : public BSExtraData
{
public:
	ExtraRefractionProperty();
	virtual ~ExtraRefractionProperty();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraStartingWorldOrCell : public BSExtraData
{
public:
	ExtraStartingWorldOrCell();
	virtual ~ExtraStartingWorldOrCell();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraHotkey : public BSExtraData
{
public:
	ExtraHotkey();
	virtual ~ExtraHotkey();

	UInt8	index;		// 00C (is 0-7)
	static ExtraHotkey* Create();
};

// 030
class ExtraEditorRefMovedData : public BSExtraData
{
	ExtraEditorRefMovedData();
	virtual ~ExtraEditorRefMovedData();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
	UInt32	unk020;		// 020
	UInt32	unk024;		// 024
	UInt32	unk028;		// 028
	UInt32	unk02C;		// 02C
};
STATIC_ASSERT(sizeof(ExtraEditorRefMovedData) == 0x30);

// 010
class ExtraInfoGeneralTopic : public BSExtraData
{
public:
	ExtraInfoGeneralTopic();
	virtual ~ExtraInfoGeneralTopic();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraHasNoRumors : public BSExtraData
{
public:
	ExtraHasNoRumors();
	virtual ~ExtraHasNoRumors();

	UInt32	unk00C;		// 00C
};

// 018
class ExtraSound : public BSExtraData
{
	ExtraSound();
	virtual ~ExtraSound();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};



// 010
class ExtraLinkedRef : public BSExtraData
{
public:
	ExtraLinkedRef();
	virtual ~ExtraLinkedRef();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraLinkedRefChildren : public BSExtraData
{
	ExtraLinkedRefChildren();
	virtual ~ExtraLinkedRefChildren();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 018
class ExtraActivateRef : public BSExtraData
{
	ExtraActivateRef();
	virtual ~ExtraActivateRef();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 018
class ExtraActivateRefChildren : public BSExtraData
{
	ExtraActivateRefChildren();
	virtual ~ExtraActivateRefChildren();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 010
class ExtraTalkingActor : public BSExtraData
{
public:
	ExtraTalkingActor();
	virtual ~ExtraTalkingActor();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraObjectHealth : public BSExtraData
{
public:
	ExtraObjectHealth();
	virtual ~ExtraObjectHealth();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraDecalRefs : public BSExtraData
{
	ExtraDecalRefs();
	virtual ~ExtraDecalRefs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraCellImageSpace : public BSExtraData
{
public:
	ExtraCellImageSpace();
	virtual ~ExtraCellImageSpace();

	TESImageSpace* imageSpace;		// 00C
};

// 014
class ExtraNavMeshPortal : public BSExtraData
{
	ExtraNavMeshPortal();
	virtual ~ExtraNavMeshPortal();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 014
class ExtraModelSwap : public BSExtraData
{
	ExtraModelSwap();
	virtual ~ExtraModelSwap();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraRadius : public BSExtraData
{
public:
	ExtraRadius();
	virtual ~ExtraRadius();

	float	radius;		// 00C
};

// 010
class ExtraRadiation : public BSExtraData
{
public:
	ExtraRadiation();
	virtual ~ExtraRadiation();

	float	radiation;		// 00C
};

// 010
class ExtraFactionChanges : public BSExtraData
{
public:
	ExtraFactionChanges();
	virtual ~ExtraFactionChanges();

	UInt32	unk00C;		// 00C
};

// 030
class ExtraDismemberedLimbs : public BSExtraData
{
	ExtraDismemberedLimbs();
	virtual ~ExtraDismemberedLimbs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
	UInt32	unk020;		// 020
	UInt32	unk024;		// 024
	UInt32	unk028;		// 028
	UInt32	unk02C;		// 02C
};
STATIC_ASSERT(sizeof(ExtraDismemberedLimbs) == 0x30);

// 010
class ExtraMultiBound : public BSExtraData
{
public:
	ExtraMultiBound();
	virtual ~ExtraMultiBound();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraMultiBoundData : public BSExtraData
{
public:
	ExtraMultiBoundData();
	virtual ~ExtraMultiBoundData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraMultiBoundRef : public BSExtraData
{
public:
	ExtraMultiBoundRef();
	virtual ~ExtraMultiBoundRef();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraReflectedRefs : public BSExtraData
{
	ExtraReflectedRefs();
	virtual ~ExtraReflectedRefs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 014
class ExtraReflectorRefs : public BSExtraData
{
	ExtraReflectorRefs();
	virtual ~ExtraReflectorRefs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraEmittanceSource : public BSExtraData
{
public:
	ExtraEmittanceSource();
	virtual ~ExtraEmittanceSource();

	UInt32	unk00C;		// 00C
};

// 01C
class ExtraRadioData : public BSExtraData
{
	ExtraRadioData();
	virtual ~ExtraRadioData();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
};
STATIC_ASSERT(sizeof(ExtraRadioData) == 0x1C);

// 010
class ExtraCombatStyle : public BSExtraData
{
public:
	ExtraCombatStyle();
	virtual ~ExtraCombatStyle();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraPrimitive : public BSExtraData
{
public:
	ExtraPrimitive();
	virtual ~ExtraPrimitive();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraOpenCloseActivateRef : public BSExtraData
{
public:
	ExtraOpenCloseActivateRef();
	virtual ~ExtraOpenCloseActivateRef();

	UInt32	unk00C;		// 00C
};

// 014
class ExtraAmmo : public BSExtraData
{
	ExtraAmmo();
	virtual ~ExtraAmmo();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 010
class ExtraPatrolRefData : public BSExtraData
{
public:
	ExtraPatrolRefData();
	virtual ~ExtraPatrolRefData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraPackageData : public BSExtraData
{
public:
	ExtraPackageData();
	virtual ~ExtraPackageData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraOcclusionPlane : public BSExtraData
{
public:
	ExtraOcclusionPlane();
	virtual ~ExtraOcclusionPlane();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraCollisionData : public BSExtraData
{
public:
	ExtraCollisionData();
	virtual ~ExtraCollisionData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraSayTopicInfoOnceADay : public BSExtraData
{
public:
	ExtraSayTopicInfoOnceADay();
	virtual ~ExtraSayTopicInfoOnceADay();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraEncounterZone : public BSExtraData
{
public:
	ExtraEncounterZone();
	virtual ~ExtraEncounterZone();

	UInt32	unk00C;		// 00C
};

// 018
class ExtraSayToTopicInfo : public BSExtraData
{
	ExtraSayToTopicInfo();
	virtual ~ExtraSayToTopicInfo();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 010
class ExtraOcclusionPlaneRefData : public BSExtraData
{
public:
	ExtraOcclusionPlaneRefData();
	virtual ~ExtraOcclusionPlaneRefData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraPortalRefData : public BSExtraData
{
public:
	ExtraPortalRefData();
	virtual ~ExtraPortalRefData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraPortal : public BSExtraData
{
public:
	ExtraPortal();
	virtual ~ExtraPortal();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraRoom : public BSExtraData
{
public:
	ExtraRoom();
	virtual ~ExtraRoom();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraHealthPerc : public BSExtraData
{
public:
	ExtraHealthPerc();
	virtual ~ExtraHealthPerc();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraRoomRefData : public BSExtraData
{
public:
	ExtraRoomRefData();
	virtual ~ExtraRoomRefData();

	UInt32	unk00C;		// 00C
};

// 01C
class ExtraGuardedRefData : public BSExtraData
{
	ExtraGuardedRefData();
	virtual ~ExtraGuardedRefData();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
};

// 018
class ExtraCreatureAwakeSound : public BSExtraData
{
	ExtraCreatureAwakeSound();
	virtual ~ExtraCreatureAwakeSound();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 020
class ExtraWaterZoneMap : public BSExtraData
{
	ExtraWaterZoneMap();
	virtual ~ExtraWaterZoneMap();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
};
STATIC_ASSERT(sizeof(ExtraWaterZoneMap) == 0x20);

// 00C
class ExtraIgnoredBySandbox : public BSExtraData
{
public:
	ExtraIgnoredBySandbox();
	virtual ~ExtraIgnoredBySandbox();
};

// 010
class ExtraCellAcousticSpace : public BSExtraData
{
public:
	ExtraCellAcousticSpace();
	virtual ~ExtraCellAcousticSpace();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraReservedMarkers : public BSExtraData
{
public:
	ExtraReservedMarkers();
	virtual ~ExtraReservedMarkers();

	UInt32	unk00C;		// 00C
};

// 018
class ExtraWeaponIdleSound : public BSExtraData
{
	ExtraWeaponIdleSound();
	virtual ~ExtraWeaponIdleSound();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 014
class ExtraWaterLightRefs : public BSExtraData
{
	ExtraWaterLightRefs();
	virtual ~ExtraWaterLightRefs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 014
class ExtraLitWaterRefs : public BSExtraData
{
	ExtraLitWaterRefs();
	virtual ~ExtraLitWaterRefs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
};

// 018
class ExtraWeaponAttackSound : public BSExtraData
{
	ExtraWeaponAttackSound();
	virtual ~ExtraWeaponAttackSound();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 018
class ExtraActivateLoopSound : public BSExtraData
{
	ExtraActivateLoopSound();
	virtual ~ExtraActivateLoopSound();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 010
class ExtraPatrolRefInUseData : public BSExtraData
{
public:
	ExtraPatrolRefInUseData();
	virtual ~ExtraPatrolRefInUseData();

	UInt32	unk00C;		// 00C
};

// 010
class ExtraAshPileRef : public BSExtraData
{
public:
	ExtraAshPileRef();
	virtual ~ExtraAshPileRef();

	UInt32	unk00C;		// 00C
};

// 018
class ExtraCreatureMovementSound : public BSExtraData
{
	ExtraCreatureMovementSound();
	virtual ~ExtraCreatureMovementSound();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
};

// 028
class ExtraFollowerSwimBreadcrumbs : public BSExtraData
{
	ExtraFollowerSwimBreadcrumbs();
	virtual ~ExtraFollowerSwimBreadcrumbs();

	UInt32	unk00C;		// 00C
	UInt32	unk010;		// 010
	UInt32	unk014;		// 014
	UInt32	unk018;		// 018
	UInt32	unk01C;		// 01C
	UInt32	unk020;		// 020
	UInt32	unk024;		// 024
};

STATIC_ASSERT(sizeof(ExtraDismemberedLimbs) == 0x30);


class ExtraTerminalState : public BSExtraData
{
public:
	ExtraTerminalState();
	virtual ~ExtraTerminalState();

	UInt8 lockedOut;
	SInt8 lockLevel;
	UInt16 pad;
};

STATIC_ASSERT(sizeof(ExtraTerminalState) == 0x10);

