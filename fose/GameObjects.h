#pragma once

#include "GameForms.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"
#include "GameSound.h"
#include "netimmerse.h"
#include "havok.h"


class AnimIdle;
struct KFModel;
struct FurnitureMark;
struct DialogueItem;
class MobileObject;
class HitData;
struct DialogueResponse;
class BipedAnim;
class Sky;
class BSFile;
struct AnimData;

enum
{
	kSitSleepState_Normal = 0x0,
	kSitSleepState_LoadSitIdle = 0x1,
	kSitSleepState_WantToSit = 0x2,
	kSitSleepState_WaitingForSitAnim = 0x3,
	kSitSleepState_Sitting = 0x4,
	kSitSleepState_WantToStand = 0x5,
	kSitSleepState_LoadingSleepIdle = 0x6,
	kSitSleepState_WantToSleep = 0x7,
	kSitSleepState_WaitingForSleepAnim = 0x8,
	kSitSleepState_Sleeping = 0x9,
	kSitSleepState_WantToWake = 0xA,
};


class Actor;
class MagicCaster;
class ActiveEffect;

// 10
class MagicTarget
{
public:
	virtual bool AddTarget(MagicCaster* apCaster, MagicItem* apItem, ActiveEffect* apEffect, bool);
	virtual TESObjectREFR* GetTargetStatsObject();
	virtual tList<ActiveEffect>* GetActiveEffectList();
	virtual bool MagicTargetIsActor();
	virtual bool IsInvulnerable();
	virtual void EffectAdded(ActiveEffect* apEffect);
	virtual void EffectRemoved(ActiveEffect* apEffect);
	virtual void EffectAbsorbed(MagicCaster* apCaster, ActiveEffect* apEffect);
	virtual void EffectReflected(MagicCaster* apCaster, ActiveEffect* apEffect);
	virtual float CheckResistance(MagicCaster* apCaster, MagicItem* apItem, ActiveEffect* apEffect);
	virtual bool CheckAbsorb(MagicCaster* apCaster, MagicItem* apItem, ActiveEffect* apEffect, bool);
	virtual bool CheckReflect(MagicCaster* apCaster, MagicItem* apItem, ActiveEffect* apEffect);

	struct SpellInfo
	{
		MagicItem* magItem;
		MagicCaster* magCaster;
		ActiveEffect* activeEff;
	};

	UInt8 byte04; // 04
	UInt8 byte05; // 05
	UInt8 pad06[2]; // 06
	tList<SpellInfo> spellInfos; // 08

	void RemoveEffect(EffectItem* effItem);
};

static_assert(sizeof(MagicTarget) == 0x10);

class TESPackageData
{
public:
	TESPackageData();
	~TESPackageData();
	virtual void Destroy(bool free);
	virtual void CopyFrom(TESPackageData* packageData);
	virtual void Unk_02(void);
	virtual void Save(ModInfo* modInfo);
	virtual void Unk_04(void);
	virtual void Unk_05(void);
	virtual void Unk_06(void);
	virtual void Unk_07(void);
};

enum PACKAGE_OBJECT_TYPE : UInt32
{
	POTYPE_NONE = 0x0,
	POTYPE_ACTI = 0x1,
	POTYPE_ARMO = 0x2,
	POTYPE_BOOK = 0x3,
	POTYPE_CLOT_DEPRECATED = 0x4,
	POTYPE_CONT = 0x5,
	POTYPE_DOOR = 0x6,
	POTYPE_INGR = 0x7,
	POTYPE_LIGH = 0x8,
	POTYPE_MISC = 0x9,
	POTYPE_FLOR = 0xA,
	POTYPE_FURN = 0xB,
	POTYPE_WEAP = 0xC,
	POTYPE_AMMO = 0xD,
	POTYPE_NPC_ = 0xE,
	POTYPE_CREA = 0xF,
	POTYPE_KEYM = 0x10,
	POTYPE_ALCH = 0x11,
	POTYPE_FOOD = 0x12,
	POTYPE_CLOT = 0x13,
	POTYPE_WEAR = 0x14,
	POTYPE_15 = 0x15,
	POTYPE_WEAP_MELEE = 0x16,
	POTYPE_WEAP_RANGED = 0x17,
	POTYPE_ACTOR_EFFECT = 0x18,
	POTYPE_19 = 0x19,
	POTYPE_1A = 0x1A,
	POTYPE_1B = 0x1B,
	POTYPE_ACTOR = 0x1C,
};

struct PackageInfo
{
	TESPackage* package; // 00
	TESPackageData* packageData; // 04
	TESObjectREFR* targetRef; // 08
	UInt32 iCurrentProcedureIndex; // 0C	Initialized to 0FFFFFFFFh, set to 0 on start
	float fPackageStartTime; // 10	Initialized to -1.0	. Set to GameHour on start so some time
	UInt32 flags; // 14	Flags, bit0 would be not created and initialized
};

class bhkCharacterController;

class BaseProcess
{
public:
	BaseProcess();

	struct CachedValues
	{
		enum
		{
			kCached_Radius = 0x1,
			kCached_WidthX = 0x2,
			kCached_WidthY = 0x4,
			kCached_DPS = 0x8,
			kCached_MedicineSkillMult = 0x10,
			kCached_Paralysis = 0x20,
			kCached_HealRate = 0x40,
			kCached_FatigueReturnRate = 0x80,
			kCached_PerceptionCondition = 0x100,
			kCached_EyeHeight = 0x200,
			kCached_SomethingShouldAttack = 0x400,
			kCached_WalkSpeed = 0x1000,
			kCached_RunSpeed = 0x2000,
			kCached_NoCrippledLegs = 0x4000,
			kCached_Height = 0x8000,
			kCached_IsGhost = 0x10000000,
			kCached_Health = 0x20000000,
			kCached_Fatigue = 0x40000000
		};

		float fRadius;
		float fWidth;
		float fLength;
		float fHeight;
		float fDPS;
		float fMedicineSkillMult;
		float fParalysis;
		float fHealRate;
		float fFatigueReturnRate;
		float fPerceptionCondition;
		float fEyeHeight;
		int iSetAggression;
		int uiAssistance;
		float fWalkSpeed;
		float fRunSpeedMult;
		unsigned __int8 bHasNoCrippledLegs;
		unsigned int uiFlags;
	};

	virtual ~BaseProcess();
	virtual void Copy(BaseProcess* apSource);
	virtual void HandleDeletedReference(Actor* apActor);
	virtual void UpdateAlt(MobileObject* apObject, float afDelta);
	virtual void Update(MobileObject* apObject);
	virtual void UpdateMissedPackages(MobileObject* apObject, bool abActionflag);
	virtual NiPoint3 GetCurrentPackageLocation(Actor* apActor);
	virtual NiPoint3 GetDetectionLocation(Actor* apActor, int);
	virtual double GetLastTimeDetected(Actor* apActor, int);
	virtual bool CheckForNewPackage(Actor* apActor, bool abFlag);
	virtual void ComputeLastTimeProcessed();
	virtual bool IsWandering();
	virtual uint32_t GetProcessedDay();
	virtual uint32_t GetProcessedMonth();
	virtual uint32_t GetProcessedYear();
	virtual void SetPackageEvaluateHour(int32_t aiVal);
	virtual int32_t GetPackageEvaluateHour();
	virtual bool SetupSpecialIdle(Actor* apActor, TESIdleForm* apIdle, uint32_t aeDefaultAction, bool abCrippled, bool abForce,
	                              bool abTestConditions);
	virtual void SetLastSeenLocation();
	virtual MagicItem* GetCurrentPackageSpell();
	virtual void SetCurrentPackageSpell(MagicItem* apSpell);
	virtual void PerformOneRoundofCombat(Actor* apActor, uint32_t aeProcessLevel);
	virtual void ClearFaceAnimationData();
	virtual void UpdateHealthBar(Actor* apActor);
	virtual bool GetIdleDoneOnce();
	virtual void SetIdleDoneOnce(bool abVal);
	virtual bool EvaluateDetection(Actor* apActor);
	virtual void EvaluateCombatDetectionLists(Actor* apActor);
	virtual bool ShouldRunPlayerDetection(Actor* apActor);
	virtual bool ProcessActivateOneHour(Actor apActor, float afTimeDelta);
	virtual bool IsDoingSayTo();
	virtual void ProcessSandman(Actor* apActor);
	virtual void ProcessCannibal(Actor* apActor);
	virtual void ClearFurniture(Actor* apActor);
	virtual void SetDoingSayTo(bool abVal);
	virtual bool SetTargetForPackage(Actor* apActor);
	virtual bool SetLocationForPackage(Actor* apActor);
	virtual TESObjectREFR* GetCurrentDestinationReference(Actor* apActor, bool abInterrupt);
	virtual NiPoint3* GetCurrentDestinationCoordinate(NiPoint3& arPos, Actor* apActor, bool abInterrupt);
	virtual TESObjectCELL* GetCurrentDestinationCell(Actor* apActor);
	virtual TESWorldSpace* GetCurrentDestinationWorldSpace(Actor* apActor);
	virtual float GetCurrentDestinationRadius(Actor* apActor);
	virtual MobileObject* GetDialogTarget();
	virtual void ClearDialogTarget();
	virtual bool CheckAndStartCombatControllerForPackage(Actor* apActor, TESPackage* apPackage, bool abInCombat);
	virtual bool ShouldUpdateCombatControllerOrEndCombat(Actor* apActor, TESPackage** appPackage);
	virtual float GetSocialTalkTimer();
	virtual void SetSocialTalkTimer(float afValue);
	virtual void RemoveSpokenToActor(Actor* apActor);
	virtual void ProcessRemoveWorn(Actor* apActor, bool abInterateIndex);
	virtual void CreateFollowforEscort(Actor* apActor, TESPackage* apPackage, bool abCheck);
	virtual bool GetDetectAlert();
	virtual void SetDetectAlert(bool abVal);
	virtual void RandomlyPlaySpecialIdles(Actor* apActor);
	virtual void PostAnimFreeUpSpecialIdle(Actor* apActor);
	virtual void ResetSearchTimer();
	virtual void ModEssentialDownTimer();
	virtual float GetEssentialDownTimer();
	virtual void SetEssentialDownTimer(float afValue);
	virtual void ResetSearchChatterTimer();
	virtual void* InsertIntoDetectionList(Actor* apActor, uint32_t aeDetectionLevel, bool abInLOS, int32_t aiMinDetectionLevel,
	                                      bool, bool, bool abInCombat);
	virtual void* GetDetectingActors();
	virtual void* GetActorsDetectionEvent(Actor* apActor);
	virtual void SetActorsDetectionEvent(Actor* apActor, NiPoint3 akPosition, uint32_t auiSoundLevel, int unused,
	                                     TESObjectREFR* apRef);
	virtual void RemoveDetectionEvent();
	virtual void CheckForExpiredDetectionEvent();
	virtual void GetWeaponScore();
	virtual void SetWeaponScore(float afVal);
	virtual bool GetPickpocketed();
	virtual void SetPickpocketed();
	virtual void SetCurrentActionComplete(bool abVal);
	virtual bool GetCurrentActionComplete();
	virtual void SetCurrentMovementComplete(bool abVal);
	virtual bool GetCurrentMovementComplete();
	virtual TESObjectREFR* GetTarget();
	virtual void SetTarget(TESObjectREFR* apTarget);
	virtual TESObjectREFR* GetDoor();
	virtual void SetDoor(TESObjectREFR* apDoor);
	virtual void SetAcquireObject(void* apObject);
	virtual void* GetSavedAcquireObject();
	virtual void SetSavedAcquireObject(void* apObject);
	virtual void* GetFaceAnimationData();
	virtual ExtraContainerChanges::EntryData* GetCurrentWeapon();
	virtual ExtraContainerChanges::EntryData* GetCurrentAmmo();
	virtual bool GetLipQueued();
	virtual void SetLipQueued(bool abVal);
	virtual uint32_t GetNumberOfItemsActivated();
	virtual void SetNumberOfItemsActivated(uint32_t auiCount);
	virtual bool SetCurrentWeapon(ExtraContainerChanges::EntryData* apWeapon, NiNode* apScene, bool abForce);
	virtual void SetCurrentTorch(ExtraContainerChanges::EntryData* apTorch);
	virtual bool SetCurrentAmmo(ExtraContainerChanges::EntryData* apAmmo);
	virtual void SetCurrentShield(ExtraContainerChanges::EntryData* apShield);
	virtual void ResolveEquipmentQueue(Actor* apActor);
	virtual bool IsItemQueued(TESBoundObject* apItem, ExtraDataList* apExtraList, bool abEquip);
	virtual void AddQueuedItem(Actor* apActor, bool abEquip, TESBoundObject* apItem, int32_t aiEquip, ExtraDataList* apExtraList,
	                           bool abNow, bool abForceEquip, bool abFromMenus, bool abForceUnequip, bool abEquipCalling,
	                           bool abPlaySound);
	virtual void RemoveQueuedItem(TESBoundObject* apObject, ExtraDataList* apExtraList);
	virtual void RemoveAllQueuedItems();
	virtual NiNode* GetFireNode();
	virtual void SetFireNode(NiAVObject* apNode);
	virtual bool StoreBonePointers(NiControllerManager* apControllerManager, NiNode* apNode);
	virtual NiNode* GetWeaponBone(BipedAnim* apAnim);
	virtual NiNode* GetTorchBone(BipedAnim* apAnim);
	virtual NiNode* GetShieldBone(BipedAnim* apAnim);
	virtual NiNode* GetQuiverBone(BipedAnim* apAnim);
	virtual NiNode* GetRightWeaponBone(BipedAnim* apAnim);
	virtual NiNode* GetLeftWeaponBone(BipedAnim* apAnim);
	virtual bool IsCurrentWeaponGrenade();
	virtual bool IsCurrentWeaponMine();
	virtual bool IsCurrentWeaponGrenadeOrMine();
	virtual AnimData* GetAnimation();
	virtual bool GetLockedLocation();
	virtual void SetLockedLocation(bool abVal);
	virtual bool GetIsAggressor();
	virtual void SetIsAggressor(bool abVal);
	virtual void ForceWeaponDrawnSheathed(bool abDrawn, BipedAnim* apBipedAnim, AnimData* apAnim, Actor* apActor);
	virtual void SetItemBeingUsed(TESObject* apItem);
	virtual TESObject* GetItemBeingUsed();
	virtual void UpdateReparentWeapon(Actor* apActor);
	virtual void CreateFollownoEscort(Actor* apActor, bool abFlag);
	virtual float GetAwarePlayerTimer();
	virtual void ModAwarePlayerTimer(float afVal);
	virtual void SetCombatDelayFadeTimer(float afVal);
	virtual bool CheckCombatDelayTimer();
	virtual void AddTalkedtoActor(Actor* apActor);
	virtual bool ComputeCanInitiateDialogueWithNPC(Actor* apActor, Actor* apTarget);
	virtual bool ComputeCanRespondToDialogueFromNPC(Actor* apActor, Actor* apTarget);
	virtual bool IsActorInsideAmbushArea(Actor* apActor, Actor* apTarget);
	virtual bool CheckIfThereSomeoneTalkWith(Actor* apActor);
	virtual bool ComputeAllowSandboxConversation(Actor* apActor, Actor* apTarget);
	virtual float GetPotentialThreatDamage(Actor* apActor, Actor* apTarget);
	virtual TESPackage* GetRunOncePackage();
	virtual void SetRunOncePackage(TESPackage* apPackage, Actor* apActor);
	virtual void ClearRunOncePackage();
	virtual void SetRunOnceProcedureIndex(int32_t aiIndex);
	virtual int32_t GetRunOnceProcedureIndex();
	virtual void AddToRunOnceProcedureIndex(int32_t aiOffset);
	virtual void ClearGreetingInfoData();
	virtual bool IsPackageDone(Actor* apActor);
	virtual TESPackage* GetCurrentPackage();
	virtual void SetCurrentPackage(TESPackage* apPackage, Actor* apActor);
	virtual void ClearCurrentPackage();
	virtual void SetCurrentProcedureIndex(int32_t aiIndex);
	virtual int32_t GetCurrentProcedureIndex();
	virtual void AddToCurrentProcedureIndex(int32_t aiOffset);
	virtual void SetupNewPackage();
	virtual DialogueItem* GetSubtitleItem();
	virtual void SetSubtitleItem(DialogueItem* apSubtitle);
	virtual void AddAvoidPathingArea(Actor* apActor, NiPoint3 akPoint, float afRadius, float afTimeToExpire,
	                                 float afRadiationLevel, TESObjectREFR* apRef, TESObjectSTAT* apMarker);
	virtual void* CreateAvoidArray();
	virtual bool IsAvoidAreaInAvoidPathingList(TESObjectREFR* apRef);
	virtual void SetAvoidWaitTimer(float afVal);
	virtual void RemoveAvoidPathingNode(TESObjectREFR* apRef);
	virtual float GetHeadTrackChangeTimer();
	virtual void SetHeadTrackChangeTimer(float afVal);
	virtual void ClearLastHeadTrackTarget();
	virtual TESPackageData* GetRunOncePackageInstanceData();
	virtual TESPackageData* GetInstanceDataThatIsRunning();
	virtual PackageInfo* GetActorPackageThatIsRunning();
	virtual TESPackage* GetPackageThatIsRunning();
	virtual SInt32 GetProcedureIndexRunning();
	virtual void SetProcedureIndexRunning(SInt32 aiIndex);
	virtual void AddToProcedureIndexRunning(Actor* apActor, SInt32 aiOffset);
	virtual bhkCharacterController* GetCharacterController();
	virtual void SetCharController(NiPointer<bhkCharacterController>& arController);
	virtual void EndMoveMessage(Actor* apActor);
	virtual void ProcessFollow(Actor* apActor, bool abFlag, UInt32 aeMoveMode, bool abPursuit);
	virtual void ProcessPursue(Actor* apActor, bool abNewPackage, bool abInterateIndex, UInt32 aeMoveMode);
	virtual void ProcessFlee(Actor* apActor);
	virtual void ProcessGreet(Actor* apActor, TESTopic* apTopic, bool abForceSub, bool abStop, bool abQueue, bool abSayCallBack);
	virtual void ProcessObserveCombat(Actor* apActor);
	virtual void ProcessGetUpOneHour(Actor* apActor);
	virtual void SitOrSleep(Actor* apActor);
	virtual bool ShouldDisableCharController(TESObjectREFR* apFurnitureRef, UInt32 auiFurnitureType);
	virtual bool ProcessGetUp(Actor* apActor);
	virtual void ProcessTravel(Actor* apActor, bool abNewPackage, bool abIterateIndex, UInt32 aeMoveMode, bool abSecondLoc);
	virtual void* GetWadingWaterData();
	virtual void RemoveDetectionActor(Actor* apActor, UInt32 aeRemoveType);
	virtual void SetDetectionTimer();
	virtual void GetLineSightonActor(Actor* apActor, Actor* apTarget, UInt32 aeDetectMode, bool abFullTest);
	virtual bool Get360LineSightonActor(Actor* apActor, Actor* apTarget, UInt32 aeDetectMode);
	virtual SInt32 GetDetectionActor(Actor* apTarget, UInt32 aeDetectMode);
	virtual bool GetTalkingtoPC();
	virtual void SetTalkingtoPC(bool abVal);
	virtual void UpdateFollowers(Actor* apActor, float afTimeDelta);
	virtual int GetEmotionsDispostion(Actor* apActor, TESObjectREFR* apTarget);
	virtual UInt32 GetLastSpeakingEmotion();
	virtual void SetLastSpeakingEmotion(UInt32 aeEmotion);
	virtual void RunDetection();
	virtual double GetDetectionTimer();
	virtual void ReduceDetectionTimer();
	virtual void SetBreathTimer(float afVal);
	virtual float GetBreathTimer();
	virtual void SetSurfacing(bool abVal);
	virtual bool GetSurfacing();
	virtual bool GetGreetingFlag();
	virtual void SetGreetingFlag(bool abVal);
	virtual float GetSoundDelay();
	virtual void SetSoundDelay(float afVal);
	virtual bool GetAlert();
	virtual void SetAlert(bool abVal);
	virtual bool GetFinishingCombatPackage();
	virtual void SetFinishingCombatPackage(bool abVal);
	virtual ExtraContainerChanges::EntryData* CreateWeaponAcquireList(Actor* apActor, UInt32 aePackageObjectType);
	virtual void ClearAcquireList();
	virtual float GetIdleTimer();
	virtual void SetIdleTimer(float afValue);
	virtual bool EnterCombat(Actor* apActor, Actor* apTarget, bool, bool abIsEvil, void* apCombatGroup, bool, bool,
	                         bool abIsAggressor, bool, bool abIgnoreActorLimit, bool, bool, TESPackage* apPackage);
	virtual void GuardCallforHelp(Actor* apActor, Actor* apCriminal);
	virtual void CallForHelp(Actor* apActor, Actor* apCriminal, void* apCrime);
	virtual void SetNumberGuardsArresting(SInt32 aiVal);
	virtual UInt32 CalculateMoveMode(Actor* apActor, float afDistanceSq, float afShortDistSq, float afLongDistSq,
	                                 bool abAlarmOrCombat, bool abPureTravel);
	virtual void SetActorsAnimation(Actor* apActor, UInt32 aeMoveMode, bool abCheckAlwaysRun);
	virtual void FinishDying(Actor* apActor);
	virtual bool ProcessStandOff(Actor* apActor, float afRadius, UInt32 aeMoveMode);
	virtual bool IsRunningRunOnce();
	virtual bool IsAFollower();
	virtual void SetFollower(bool abVal);
	virtual SInt32 GetWeaponCondition(Actor* apActor);
	virtual bool GetWaitingforLipFile();
	virtual void SetWaitingforLipFile(bool abVal);
	virtual void SetLipFileFailed(bool abVal);
	virtual bool GetLipFileFailed();
	virtual void* GetLipAnim();
	virtual void SetLipAnim(void* apAnim);
	virtual void SetPlantedExplosive(bool abVal);
	virtual bool GetPlantedExplosive();
	virtual TESIdleForm* GetLastIdlePlayed();
	virtual void SetLastPlayedIdle(TESIdleForm* idleForm);
	virtual void FreeUpSpecialIdle(Actor* apActor);
	virtual SInt32 GetActorValueI(TESActorBase* apBase, UInt32 aeIndex, Actor* apActor);
	virtual float GetActorValueF(TESActorBase* apBase, UInt32 aeIndex, Actor* apActor);
	virtual void TempModActorValueF(Actor* apActor, UInt32 aeIndex, float afVal);
	virtual void TempModActorValueI(Actor* apActor, UInt32 aeIndex, SInt32 aiVal);
	virtual void DamageModActorValueF(Actor* apActor, UInt32 aeIndex, float afVal);
	virtual void DamageModActorValueI(Actor* apActor, UInt32 aeIndex, SInt32 aiVal);
	virtual void SetCachedActorValueOutOfDate(UInt32 aeIndex);
	virtual void InitializeActiveEffectList();
	virtual void* GetActiveEffectList();
	virtual void DestroyActiveEffectList();
	virtual void CastAbility(MagicCaster* apCaster, SpellItem* apAbility, bool abLoadCast);
	virtual void TransferDisease(MagicCaster* apCaster, SpellItem* apDisease, MagicTarget* apTarget, bool abLoadCast);
	virtual MagicItem* GetCurrentSpell();
	virtual void SetCurrentSpell(MagicItem* apSpell);
	virtual MagicTarget* GetDesiredTarget();
	virtual void SetDesiredTarget(MagicTarget* apTarget);
	virtual bool GetShouldCheckMagicNode();
	virtual void SetShouldCheckMagicNode(bool abVal);
	virtual void Move(MobileObject*, NiPoint3);
	virtual SInt16 GetAnimAction();
	virtual BSAnimGroupSequence* GetAnimActionAnimSeq();
	virtual void SetAnimAction(SInt16 aeAnimationAction, BSAnimGroupSequence* apSequence);
	virtual bool GetAutomaticFireAtLeastOnce();
	virtual void SetAutomaticFireAtLeastOnce(bool abVal);
	virtual bool CanAttack();
	virtual bool CanForceGreet(Actor* apActor);
	virtual void SetIronSights(bool isAiming);
	virtual bool GetIronSights();
	virtual bool HasMovementAnim(UInt32 aeAnim);
	virtual SInt32 GetKnockedState();
	virtual void SetKnockedState(char state);
	virtual void KnockDown(Actor* apActor);
	virtual void KnockExplosion(Actor* apActor, NiPoint3 akSource, float afForce);
	virtual void UpdateKnockState(Actor* apActor);
	virtual void ClearKnockState(Actor* apActor);
	virtual void SetAimSequence(UInt32 aeDirection, BSAnimGroupSequence* apSequence);
	virtual void UpdateAimSequences(Actor* apActor);
	virtual float GetAimLooking();
	virtual void AddGunDrift(float afValue);
	virtual void ClearAutomaticShotsToFire(bool abStartShooting);
	virtual UInt8 GetAutomaticShotsToFire(Actor* apTarget);
	virtual void SetShotsToFire(UInt8 aucCount);
	virtual UInt8 GetShotsToFire();
	virtual float GetAutomaticShotDelay();
	virtual void AttackCallback(UInt32 aiNumAttacks);
	virtual bool GetWantWeaponDrawn();
	virtual void SetWantWeaponDrawn(bool abVal);
	virtual bool GetWeaponDrawn();
	virtual void SetWeaponDrawn(Actor* apActor, bool abVal);
	virtual void SetWeaponLastPos(NiPoint3 akPos);
	virtual NiPoint3* GetWeaponLastPos();
	virtual void Update3DModel(Actor* apActor);
	virtual void Set3DUpdateFlag(uint8_t aucFlag);
	virtual void Clear3DUpdateFlag(uint8_t aucFlag);
	virtual void ClearAll3DUpdateFlags();
	virtual bool Get3DUpdateFlag(uint8_t aucFlag);
	virtual uint8_t GetAll3DUpdateFlags();
	virtual void ClearObjectFromAcquireList(TESObjectREFR* apObject);
	virtual void ClearObjectFromAcquireListAlt(uint32_t auiFormID);
	virtual TESObjectREFR* GetLastGreeted();
	virtual void SetLastGreeted(TESObjectREFR* apRef);
	virtual BSSoundHandle GetSoundHandle(uint32_t aeType);
	virtual void SetSoundHandle(uint32_t aeType, BSSoundHandle akHandle);
	virtual void StopSoundHandle(uint32_t aeType);
	virtual void StartTorchSound(Actor* apActor);
	virtual bool GetMovementStopped();
	virtual void ClearMovementStopped();
	virtual void SetDetectionModifier(float afVal);
	virtual void SetDetectionModifierTimer();
	virtual float GetDetectionModifier();
	virtual float GetGreetingTimer();
	virtual void SetGreetingTimer(float afVal);
	virtual bool IsTalking();
	virtual uint32_t GetSitSleepState();
	virtual void SetSitSleepState(Actor* apActor, uint32_t aeState, TESObjectREFR* apFurniture, uint8_t aucFurnitureIndex);
	virtual uint32_t GetFurnitureMarkerID();
	virtual TESObjectREFR* GetCurrentFurniture();
	virtual uint32_t GetCurrentFurnitureIndex();
	virtual FurnitureMark* GetFurnitureMarker();
	virtual bool LoadDynamicIdle(Actor* apActor);
	virtual void LoadSitSleepIdle(Actor* apActor, AnimData* apAnimation);
	virtual bool ContinuingPackageforPC();
	virtual void SetContinuingPackage(bool abVal);
	virtual bool IsTargetActivated();
	virtual void SetTargetActivated();
	virtual bool HasBeenAttacked();
	virtual void SetBeenAttacked(bool abVal);
	virtual void* GetLightingShaderProperty();
	virtual void SetLightingShaderProperty(void* apProp);
	virtual void CalculateLightValue(Actor* apActor, bool abIgnoreTorch);
	virtual void* GetDetectionState(Actor* apTarget, uint32_t aeListType);
	virtual void* InsertIntoDetectionArray(Actor* apTarget, uint32_t auiDetectionLevel);
	virtual double GetLightUpdateTimer();
	virtual void SetLightUpdateTimer(float afVal);
	virtual TESObjectREFR* GetGenericLocation();
	virtual void SetGenericLocation(TESObjectREFR* apLocation);
	virtual TESObjectREFR* GetSecondGenericLocation();
	virtual void SetSecondGenericLocation(TESObjectREFR* apLocation);
	virtual void ClearCurrentDataforProcess();
	virtual void SetCommandingActor(Actor* apActor);
	virtual Actor* GetCommandingActor();
	virtual void SetLastBoundWeapon(TESObjectWEAP* apWeapon);
	virtual TESObjectWEAP* GetLastBoundWeapon();
	virtual void SetPackageEndTimerValue(float afVal);
	virtual float GetPackageEndTimerValue();
	virtual void SetFurniture(TESObjectREFR* apFurnitureRef, uint8_t aucMarkerIndex, FurnitureMark* apMark);
	virtual void RemoveAndAttachItems(MobileObject* apObject);
	virtual uint16_t GetSaveSize(uint32_t auiFlags, MobileObject* apObject);
	virtual void SaveGameBGS(void* apBuffer);
	virtual void SaveGameTES(uint32_t auiFlags, MobileObject* apObject);
	virtual void LoadGameBGS(void* apBuffer);
	virtual void LoadGameTES(uint32_t auiFlags, uint32_t auiCurrentFlags, MobileObject* apObject);
	virtual void InitLoadGameBGS(void* apBuffer);
	virtual void InitLoadGameTES(uint32_t auiFlags, uint32_t auiOldFlags, MobileObject* apObject);
	virtual void FinishInitLoadGame(uint32_t auiFlags, uint32_t auiOldFlags, MobileObject* apObject);
	virtual void RevertBGS(void* apBuffer);
	virtual void RevertTES(uint32_t auiFlags, MobileObject* apObject);
	virtual void FinishLoadGame();
	virtual TESEffectShader GetWeaponEnchantmentVisuals();
	virtual void SetWeaponEnchantmentVisuals(TESEffectShader* apShader);
	virtual bool GetShouldRefreshMagicShadersEffects();
	virtual void SetShouldRefreshMagicShadersEffects(bool abUpdateWeaponEffectShader, bool abInitializeActiveHitEffect, bool);
	virtual void RefreshMagicShaderEffects(Actor* apActor);
	virtual void SetBeginIdlesPlayed(bool abVal);
	virtual bool GetBeginIdlesPlayed();
	virtual void SetEndIdlesPlayed(uint8_t aucVal);
	virtual bool GetEndIdlesPlayed();
	virtual void StartNewPackage(Actor* apActor, TESPackage* apPackage);
	virtual TESPackage* ChangePackage(Actor* apActor, TESPackage* apPackage);
	virtual void PackageDone(Actor* apActor, TESPackage* apPackage);
	virtual bool IsPackageDoneOnce();
	virtual void SetPackageDoneOnce(bool abVal);
	virtual float GetAlphaMult();
	virtual void SetAlphaMult(float afVal);
	virtual float GetScriptRefractionPower();
	virtual void SetScriptRefractionPower(float afVal);
	virtual void UpdateWadingWater(Actor* apActor);
	virtual int32_t GetNumberGuardsArresting();
	virtual void ModNumberGuardsArresting(int32_t aiVal);
	virtual bool ShouldCheckFlare(BSSimpleList<ActiveEffect*>* apEffects, uint32_t aeFlareType);
	virtual void SetRefreshFlareFlags();
	virtual void* GetLeveledSpells(Actor* apActor, bool abIncludeDiseases);
	virtual void SetHasHealingSpells(int32_t aiHasSpells);
	virtual int32_t GetHasHealingSpells();
	virtual void SetHasHealingPotions(int32_t);
	virtual int32_t GetHasHealingPotions();
	virtual float GetTemporaryModifier(uint8_t aeActorValue);
	virtual float GetPermanentModifier(uint8_t aeActorValue);
	virtual float GetDamageModifier(uint8_t aeActorValue);
	virtual void* GetBound();
	virtual void SetBound(void* apBound);
	virtual void SetForceNextUpdate(bool abVal);
	virtual bool GetForceNextUpdate();
	virtual void SetNeedTalkPlayer(bool abVal);
	virtual bool GetNeedTalkPlayer();
	virtual void* GetActorsInAggroRadius();
	virtual void NotifyGuardAboutActivation(Actor* apActor, Actor* apTarget, Actor* apAlly);
	virtual uint32_t GetFadeState();
	virtual void AddPostAnimationAction(uint32_t auiFlag);
	virtual uint32_t GetPostAnimationActions();
	virtual void ClearPostAnimationActions();
	virtual void RemovePostAnimationAction(uint32_t auiFlag);
	virtual void SetDefaultHeadTrackTarget(TESObjectREFR* apTarget);
	virtual void SetActionHeadTrackTarget(TESObjectREFR* apTarget);
	virtual void SetScriptHeadTrackTarget(TESObjectREFR* apTarget);
	virtual void SetCombatHeadTrackTarget(TESObjectREFR* apTarget);
	virtual void SetDialogHeadTrackTarget(TESObjectREFR* apTarget);
	virtual void SetUseWeaponHeadTrackTarget(TESObjectREFR* apTarget);
	virtual void SetHeadTrackTarget(UInt32 aeType, TESObjectREFR* apTarget);
	virtual void ClearDefaultHeadTrackTarget();
	virtual void ClearActionHeadTrackTarget(bool abHoldDefault);
	virtual void ClearScriptHeadTrackTarget(bool abHoldDefault);
	virtual void ClearCombatHeadTrackTarget();
	virtual void ClearDialogHeadTrackTarget(bool abHoldDefault);
	virtual void ClearUseWeaponHeadTrackTarget(bool abHoldDefault);
	virtual void ModDetectionModifierTimer();
	virtual float GetDetectionModifierTimer();
	virtual void ResetHeadTrackingTarget();
	virtual void RemoveHeadTrackTarget(TESObjectREFR* apTarget);
	virtual bool CanSetDefaultHeadTrackTarget();
	virtual bool CanSetActionHeadTrackTarget();
	virtual bool CanSetDialogHeadTrackTarget();
	virtual TESObjectREFR* GetCurrentHeadTrackTargetByType(UInt32 aeType);
	virtual TESObjectREFR* GetCurrentHeadTrackTarget();
	virtual uint32_t GetCurrentHeadTrackType();
	virtual const char* GetCurrentHeadTrackTypeString();
	virtual void SetForceRotate(bool abVal);
	virtual bool GetForceRotate();
	virtual TESObjectREFR* GetLastHeadTrackTarget();
	virtual bool IsInLastDefaultTargetArray(TESObjectREFR* apRef);
	virtual void SetIsSummonedCreature(bool abVal);
	virtual bool GetIsSummonedCreature();
	virtual float GetDeathTime();
	virtual void SetDeathTime(float afVal);
	virtual void AddAttachedArrow(void* apArrow);
	virtual void RemoveAttachedArrow(void* apArrow);
	virtual void MoveAttachedArrowsToParentCell(TESObjectREFR* apRef);
	virtual void* GetAttachedArrowList();
	virtual void CreateMuzzleFlash(void* apProjectile, TESObjectREFR* apShooter);
	virtual void* GetCurrentMuzzleFlash();
	virtual bool GetEnableMuzzleFlash();
	virtual void EnableMuzzleFlash();
	virtual void ClearMuzzleFlash();
	virtual void StoreDamageNodes(NiNode* apNode, BGSBodyPartData* apBodyPartData);
	virtual void StoreDismemberedLimbNode(uint32_t aeBodyPart, NiNode* apNode);
	virtual NiNode* GetDamageNode(uint32_t aeBodyPart);
	virtual NiNode* GetHeadNode();
	virtual NiNode* GetTorsoNode();
	virtual bool GetSustainedFire();
	virtual void SetSustainedFire(bool abVal);
	virtual void UpdateArmorWorn();
	virtual void IsWearingHeavyArmor();
	virtual bool IsWearingPowerArmorTorso();
	virtual bool IsWearingPowerArmorHelmet();
	virtual void SetCurrentlyReanimating(bool abVal);
	virtual bool GetCurrentlyReanimating();
	virtual void UpdateRadiation(Actor* apActor, float afTimeDelta);
	virtual void TryReserveProjectileClones(Actor* apActor);
	virtual void SetAnimActionSuccess(int32_t);
	virtual void FinishSetupSpecialIdle(Actor* apActor);
	virtual void SavePackageToExtraData(TESObjectREFR* apRef);
	virtual void LoadPackageFromExtraData(TESObjectREFR* apRef);
	virtual TESIdleForm* GetCurrentProcessIdle();
	virtual void SetCurrentProcessIdle(TESIdleForm* apIdleForm);
	virtual void SetCurrentProcessAnimIdleLoaded(AnimIdle* apAnimIdle, KFModel* apAnimIdleKF);
	virtual AnimIdle* GetCurrentProcessAnimIdle();
	virtual KFModel* GetCurrentProcessAnimIdleKF();
	virtual void ModTrackedDamage(Actor* apSource, float afDamage);
	virtual float GetTrackedDamage();
	virtual float GetActorLightLevel();
	virtual void SetActorLightLevel(float afLightAmount);
	virtual void DeferHideDismemberedLimb(uint32_t aeLimbIndex, NiNode* apDismemberedLimbRoot, NiAVObject* apReplacementLimb,
	                                      bool abExplosion);
	virtual void UpdateDeferredHideDismemberedLimbs(Actor* apActor, float afTimeDelta);
	virtual void SetCreatureLipSynchAnim(void* apAnim);
	virtual void* GetCreatureLipSynchAnim();
	virtual void SetCreatureLipSynchStartTime(uint32_t auiTime);
	virtual uint32_t GetCreatureLipSynchStartTime();
	virtual void AddRadiationMagicDelta(float afVal);
	virtual void RemoveRadiationMagicDelta(float afVal);
	virtual float GetRadiationMagicDelta();
	virtual void SetRadiationWaterDelta(float afVal);
	virtual float GetRadiationWaterDelta();
	virtual void SetRadiationDelta(float afVal);
	virtual float GetRadiationDelta();
	virtual HitData* GetLastHitData();
	virtual void SetLastHitData(HitData* apHitData);
	virtual void ClearLastHitData();
	virtual uint32_t GetWeaponConditionStage();
	virtual void SetWeaponConditionStage(uint32_t auiHealth);
	virtual void* GetFaceNode();
	virtual void SetFaceNode(void* apNode);
	virtual void* GetFaceSkinnedNode();
	virtual void SetFaceSkinnedNode(void* apNode);
	virtual NiAVObject* GetHeadAnims();
	virtual void SetHeadAnims(NiAVObject* apObject);
	virtual void CheckCachedValueFlags(Actor* apActor);
	virtual void SetPathLookAtTarget(TESObjectREFR* apTarget);
	virtual void ClearPathLookTarget(TESObjectREFR* apTarget);

	PackageInfo currentPackage;
	float fSystemTimeClock;
	float fHourLastProcessed;
	UInt32 iDateLastProcessed;
	UInt32 uiProcessLevel;
	CachedValues* cachedValues; // 2C
};

static_assert(sizeof(BaseProcess) == 0x30);

struct ActorValueModifier
{
	UInt8 actorValue;
	UInt8 pad[3];
	float fValue;
};

struct ActorValueModifiers
{
	tList<ActorValueModifier*> avMods;
	UInt8 unk008;
	UInt8 pad009[3];
	void** modifiedAV;
};

class LowProcess : public BaseProcess
{
public:
	LowProcess();
	~LowProcess();
	virtual void Unk_1E8();
	virtual void Unk_1E9();
	virtual void Unk_1EA();
	virtual void Unk_1EB();
	virtual void Unk_1EC();
	virtual void Unk_1ED();
	virtual void Unk_1EE();
	virtual void Unk_1EF();
	virtual void Unk_1F0();
	virtual void Unk_1F1();
	virtual void Unk_1F2();
	virtual void Unk_1F3();
	virtual void Unk_1F4();
	virtual void Unk_1F5();
	virtual void Unk_1F6();
	virtual void Unk_1F7();
	virtual void Unk_1F8();
	virtual void Unk_1F9();
	virtual void Unk_1FA();
	virtual void Unk_1FB();
	virtual void Unk_1FC();
	virtual void Unk_1FD();
	virtual void Unk_1FE();
	virtual void Unk_1FF();
	virtual void Unk_200();
	virtual void Unk_201();

	struct ObjectToAcquire
	{
		TESObjectREFR* pRef;
		TESForm* form;
		UInt8 bSteal;
		UInt8 bKill;
		UInt8 gap0A[2];
		UInt32 iAmount;
		UInt32 iWantValue;
		ExtraDataList* xDataList;
		UInt32 eType;
	};

	UInt8 ucLowProcessFlags;
	UInt8 pad31[3];
	TESBoundObject* pUsedItem;
	float kTimer[2];
	Actor* pTarget;
	TESForm* pGenericLocation;
	TESForm* pSecondLocation;
	BGSListForm* pList4C;
	UInt16 usCount50;
	UInt8 gap52[2];
	TESForm* unk54;
	UInt32 uiNumberItemsActivate;
	tList<void*> kObjectList;
	ObjectToAcquire* pAcquireObject;
	void* pUnk68;
	tList<void*> kGenericLocationsList;
	tList<void*> kToAcquireList;
	tList<void*> kList7C;
	TESForm* pForm84;
	UInt32 unk88;
	PACKAGE_OBJECT_TYPE ePackageObjectType;
	TESFaction* pFaction90;
	UInt32 unk94;
	ActorValueModifiers* damageModifiers;
	UInt32 unk9C;
	UInt32 unkA0;
	float fEssentialDownTimer;
	float fGameDayDied;
	float fPlayerDamageDealt;
	bool bSkippedTimeStampForPathing;
	UInt8 gapB1;
	UInt8 gapB2;
	UInt8 gapB3;
};

static_assert(sizeof(LowProcess) == 0xB4);

class MiddleLowProcess : public LowProcess
{
public:
	MiddleLowProcess();
	~MiddleLowProcess();
	virtual void Unk_202();

	UInt32 iHourPackageEvaluated;
	ActorValueModifiers tempModifiers;
};

static_assert(sizeof(MiddleLowProcess) == 0xC8);

enum KnockState : UInt8
{
	kKnockState_Normal = 0x0,
	kKnockState_Explode = 0x1,
	kKnockState_ExplodeLeadIn = 0x2,
	kKnockState_KnockedOut = 0x3,
	kKnockState_KnockOutLeadIn = 0x4,
	kKnockState_Queued = 0x5,
	kKnockState_GettingUp = 0x6,
};

struct FurnitureMark
{
	float kPos[3];
	UInt16 usRotation;
	UInt8 eType;
	UInt8 gap0F;
};

class bhkCharacterController;

class MiddleHighProcess : public MiddleLowProcess
{
public:
	MiddleHighProcess();
	~MiddleHighProcess();
	virtual void Unk_203();
	virtual void Unk_204();
	virtual void Unk_205();
	virtual void Unk_206();
	virtual void Unk_207();
	virtual void Unk_208();
	virtual void Unk_209();
	virtual void Unk_20A();
	virtual void Unk_20B();
	virtual void Unk_20C();
	virtual void Unk_20D();
	virtual void Unk_20E();
	virtual void Unk_20F();
	virtual void Unk_210();
	virtual void Unk_211();
	virtual void Unk_212();
	virtual void Unk_213();
	virtual void Unk_214();
	virtual void Unk_215();
	virtual void Unk_216();

	TESForm** unk0C8[2];
	tList<void> furnitureRefList0D0;
	float fPursueTimer;
	float unk0DC;
	UInt8 bIdleDoneOnce;
	UInt8 byte0E1;
	UInt8 byte0E2;
	UInt8 byte0E3;
	PackageInfo kRunOncePackage;
	float kLastSeenTargetPosition[3];
	PACKAGE_OBJECT_TYPE eUseItem;
	TESIdleForm* lastPlayedIdle;
	UInt8 bResetPackageIdleTimer;
	void* pWeaponInfo;
	void* pAmmoInfo;
	void* spQueuedFile;
	void* spCloneReserver;
	UInt8 bIsUsingOneHandGrenade;
	UInt8 bIsUsingOneHandMine;
	UInt8 isWearingHeavyArmor;
	UInt8 isWearingPowerArmorTorso;
	UInt8 isWearingPowerArmorHelmet;
	NiNode* pWeaponNode;
	void* class130;
	UInt8 wantsWeaponOut;
	bool bIsWeaponOut;
	UInt8 byt136;
	UInt8 byt137;
	bhkCharacterController* spCharCtrl;
	KnockState eKnockedState;
	UInt8 ucSitSleepState;
	TESObjectREFR* pFurnitureRef;
	UInt8 ucMarkerIndex;
	FurnitureMark kFurnitureMark;
	Actor* commandingActor;
	UInt32 unk15C;
	MagicItem* magicItem160;
	UInt32 unk164;
	UInt8 byte168;
	UInt32 pEnchantEffectShader;
	float fActorAlpha;
	float unk174;
	void* pFaceAnimationData;
	UInt8 byte17C;
	UInt8 byte17D;
	UInt8 byte17E;
	UInt8 byte17F;
	UInt8 byte180;
	UInt32 flags184;
	UInt8 hasCaughtPlayerPickpocketting;
	UInt8 byte189;
	UInt8 bHasLifeDetectedEffect;
	UInt8 byte18B;
	UInt8 flags18C;
	UInt8 bForceNextUpdate;
	UInt32 iPackageIdleNumber;
	TESIdleForm* pCurrentIdle;
	float fPackageIdleTimer;
	UInt8 byte19C;
	UInt8 byte19D;
	UInt32 lipSyncAnim1A0;
	UInt32 unk1A4;
	UInt32 iReservationSlot;
	void* pArrowProjectiles;
	tList<void> deferredLimbHideList;
	void* effectsList;
	MagicTarget* magicTarget1BC;
	void* pAnimData;
	void* weaponSequence[3];
	float angle1D0;
	float fTime1D4_WeaponRelated;
	UInt8 byte1D8;
	UInt8 isFiringAutomaticWeapon;
	UInt8 bGetAttacked;
	UInt8 gap1DB;
	NiNode* limbNodes[15];
	NiNode* hairBodyPartNode;
	NiNode* headBodyPartNode;
	void* spLightingProperty;
	void* boundingBox;
	UInt8 isAiming;
	UInt8 byte229;
	UInt16 byte22A_shouldEnterFurniture;
	UInt32 unk22C;
	void* queuedEquipItems;
	float unk234;
	float rads238;
	float fWaterRadsSec;
	void* lastHitData;
	UInt32 unk244;
	void* unk248;
	void* unk24C;
	void* unk250;
};

static_assert(sizeof(MiddleHighProcess) == 0x254);

struct DialogueItem;

class HighProcess : public MiddleHighProcess
{
public:
	HighProcess();
	~HighProcess();
	virtual void Unk_217();

	enum FADE_STATE : UInt32
	{
		FADE_NORMAL = 0x0,
		FADE_IN = 0x1,
		FADE_OUT = 0x2,
		FADE_TELEPORT_IN = 0x3,
		FADE_TELEPORT_OUT = 0x4,
		FADE_OUT_DISABLE = 0x5,
		FADE_OUT_DELETE = 0x6,
		FADE_MOVE_TO_MARKER = 0x7,
		FADE_STATE_COUNT = 0x8,
	};

	tList<void>* detectedActors;
	tList<void>* detectingActors;
	tList<void>* actorList264;
	tList<void>* detectionDataList268;
	tList<void>* detectionDataList26C;
	UInt8 byte270;
	tList<void> combatTargetList274;
	tList<void> combatList27C;
	tList<void> combatList284_UNUSED;
	tList<void> exteriorSpectatorsList;
	float fDetectListTimer;
	float idleChatterCommentTimer;
	UInt8 byte29C;
	UInt8 bProcessGreetSayTo;
	UInt8 gap29E;
	UInt8 gap29F;
	float fAITalkToSameNPCTimer;
	Actor* unk2A4;
	float flt2A8;
	TESObjectREFR* ref2AC;
	float fAlpha;
	float fPackageEvalTimer;
	float fUseItemTimer;
	float flt2BC;
	UInt16 count2C0;
	UInt16 numVolleyShots;
	UInt16 count2C4;
	UInt8 byte2C6;
	UInt8 bSkippedUpdate;
	float fSocialTimerForConversations;
	UInt32 unk2CC;
	float flt2D0;
	float flt2D4;
	float flt2D8;
	UInt32 unk2DC;
	float fEvaluateAcquireTimer;
	void* pBoneLODController;
	SInt32 iLODLevel;
	SInt16 usCurrentAction;
	void* pCurrentSequence;
	UInt8 bForceFireWeapon;
	float fDetectionTimer;
	UInt32 unk2FC[5];
	float time310;
	void* kSounds[6];
	UInt8 bGreetingFlag;
	UInt8 gap32D;
	UInt8 gap32E;
	UInt8 gap32F;
	float fDyingTimer;
	float fIdleTimer;
	float flt338;
	float fDiveBreath;
	UInt32 unk340;
	float painSoundTimer344;
	UInt8 byte348;
	UInt8 bWeaponAlertDrawn;
	UInt8 gap34A;
	UInt8 gap34B;
	float fAwarePlayerTimer;
	TESIdleForm* forcedIdleForm;
	void* spUnk354[2];
	void* spUnk35C[2];
	UInt8 bInDialogMenu;
	DialogueItem* pGreetTopic;
	UInt32 unk36C;
	Actor* pDialogTarget;
	UInt8 isContinuingPackagePCNear;
	UInt8 bActivateAnim;
	UInt8 gap376[2];
	float fScriptPackageEndTime;
	float flt37C;
	void* spUnk380;
	float flt384;
	float flt388;
	tList<Actor> list38C;
	tList<Actor> list394;
	UInt32 iNumberGuardsPursuing;
	UInt8 bStop;
	UInt8 gap3A1;
	UInt8 gap3A2;
	UInt8 gap3A3;
	float fReequipArmorTimer;
	UInt8 bUnequippedArmorToSwim;
	UInt32 unk3AC;
	UInt32 unk3B0;
	tList<void>* pList_3B4;
	UInt8 byte3B8;
	UInt8 byte3B9;
	UInt8 gap3BA;
	UInt8 gap3BB;
	float fDetectionModifier;
	float fDetectionModifierTimer;
	float fLightAmount;
	float fLightUpdateTimer;
	void* lipSyncAnim;
	UInt8 byte3D0;
	UInt8 byte3D1;
	UInt8 gap3D2;
	UInt8 gap3D3;
	void* muzzleFlash;
	UInt32 count3D8;
	void* pActorsGeneratedDetectionEvent;
	UInt8 byte3E0;
	FADE_STATE eFadeState;
	float fFadeAlpha;
	TESObjectREFR* pTeleportFadeRef;
	void* kHeadTracking[8];
	float fHoldHeadTrackTimer;
	TESObjectREFR* pPackageTarget;
	UInt8 byte420;
	UInt32 queuedIdleFlags;
	void* AV_428;
	float flt42C;
	UInt32 unk430;
	void* ptr434;
	float fDetectDoorsForPathingTimer;
	float fRadReevaluationTimer;
	float fRadsSec440;
	UInt8 hasLiveGrenade;
	UInt8 forceSpeakToPlayerAfterGettingUp;
	UInt8 pad446[2];
	float fTakeBackTimer; // 338
	tList<void> avoidNodes;
	DialogueItem* dialogueItem454;
	UInt8 bHiding;
	UInt8 bIsDoingSayTo;
	UInt8 gap45A;
	UInt8 gap45B;
	void* spPathingRequest0;
	void* spPathingRequest1;
	void* spPathingMessageQueue0;
	void* spPathingMessageQueue1;
};

static_assert(sizeof(HighProcess) == 0x45C);
static_assert(offsetof(HighProcess, forcedIdleForm) == 0x348);
static_assert(offsetof(HighProcess, queuedIdleFlags) == 0x414);
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
class PlayerMover;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;
class NiNode;

class TESObjectCELL;

// 004?
class TESChildCell
{
public:
	TESChildCell();
	~TESChildCell();

	// no virtual destructor
	virtual TESObjectCELL* GetChildCell(void);
};

class hkpRigidBody;
class bhkPhantom;
struct AnimData;

class TESObjectREFR : public TESForm, public TESChildCell
{
public:
	TESObjectREFR();
	~TESObjectREFR();
	virtual bool GetEditorLocation(NiPoint3& arPos, NiPoint3& arRot, TESForm*& apParentLocation, TESObjectCELL* apCell);
	virtual BSSoundHandle VoiceSoundFunction(TESTopic* apTopic, Actor* apTarget, bool ab2DSound, bool abVoice, bool, bool,
	                                         bool abForceSubtitles);
	virtual void UpdateSoundCallBack();
	virtual void DamageObject(float afDamage, bool abForce);
	virtual bool GetCastShadows();
	virtual void SetCastShadows(bool abVal);
	virtual bool GetMotionBlur();
	virtual void SetMotionBlur(bool abVal);
	virtual void IsDangerous();
	virtual bool IsObstacle();
	virtual bool IsQuestObject();
	virtual void SetActorCause(void* apActorCause);
	virtual void* GetActorCause();
	virtual NiPoint3 GetStartingAngle();
	virtual NiPoint3 GetStartingLocation();
	virtual void SetStartingPosition(NiPoint3 akPos);
	virtual void UpdateRefLight();
	virtual TESObjectREFR* RemoveItem(TESBoundObject* apObject, ExtraDataList* apExtraList, uint32_t auiCount, bool abStealing,
	                                  bool abDropWorld, TESObjectREFR* apOtherContainer, NiPoint3* apPoint, NiPoint3* apRotate,
	                                  bool abDelete, bool abPreferStolen);
	virtual void RemoveItemType(uint32_t auiTypeID, bool abStealing, uint32_t auiCount);
	virtual bool AddWornItem(TESBoundObject* apObject, uint32_t auiCount, ExtraDataList* apExtraList, bool abForceEquip = false);
	virtual bool RemoveWornItem(TESBoundObject* apObject, uint32_t auiCount, ExtraDataList* apExtraList);
	virtual void DoTrap(void*, void*);
	virtual void AddObjecttoContainer(TESBoundObject* apObject, ExtraDataList* apExtraList, uint32_t auiCount);
	virtual NiPoint3 GetLookingAtLocation();
	virtual MagicCaster* GetMagicCaster();
	virtual MagicTarget* GetMagicTarget();
	virtual bool IsChild(bool abCheckHeight);
	virtual TESActorBase* GetTemplateActorBase();
	virtual void SetTemplateActorBase(TESActorBase* apBase);
	virtual NiNode* GetFaceNodeBiped(NiNode* apNode = nullptr);
	virtual void* GetFaceNodeSkinned(NiNode* apNode = nullptr);
	virtual void* GetFaceNode(NiNode* apNode = nullptr);
	virtual void* GetFaceAnimationData(NiNode* apNode = nullptr);
	virtual bool ClampToGround();
	virtual bool DetachHavok();
	virtual void InitHavok();
	virtual NiAVObject* Load3D(bool abBackgroundLoading);
	virtual void Set3D(NiAVObject* apObject, bool abDebugPrint);
	virtual NiAVObject* Get3D();
	virtual bool HasRandomAnim();
	virtual NiPoint3 GetBoundMin();
	virtual NiPoint3 GetBoundMax();
	virtual void UpdateAnimation();
	virtual AnimData* GetAnimation();
	virtual BipedAnim* GetBiped();
	virtual BipedAnim* GetCurrentBiped();
	virtual void SetBiped(BipedAnim* apBiped);
	virtual const NiPoint3& GetLocationOnReference();
	virtual void SetRunsInLow(bool abVal);
	virtual void MoveHavok(bool abRecursive);
	virtual void SetActionComplete(bool abVal);
	virtual void SetMovementComplete(bool abVal);
	virtual void ResetInventory(bool abLeveledOnly);
	virtual NiNode* GetFireNode();
	virtual void SetFireNode(NiNode* apNode);
	virtual UInt32 GetSitSleepState();
	virtual bool IsNpc();
	virtual bool IsCreature();
	virtual bool IsExplosion();
	virtual bool IsProjectile();
	virtual void SetParentCell(TESObjectCELL* apCell);
	virtual bool IsDead(bool abNotEssential);
	virtual bool IsKnockedOut();
	virtual bool IsParalyzed();
	virtual void* CreateAnimNoteReceiver();
	virtual void* GetAnimNoteReceiever();


	enum
	{
		kFlags_Taken = 0x00000022, // 2 bits TODO: which bit does what?
		kFlags_Persistent = 0x00000400, //shared bit with kFormFlags_QuestItem
	};

	struct RenderState
	{
		TESObjectREFR* currWaterRef; // 00
		UInt32 underwaterCount; // 04	0-0x13 when fully-underwater; exterior only
		float waterLevel; // 08
		float revealDistance; // 0C
		UInt32 flags; // 10
		NiNode* rootNode; // 14
		bhkPhantom* phantom; // 18	Used with trigger volume
	};

	TESForm* baseForm; // 01C

	NiVector3 rotation; // 020 - either public or accessed via simple inline accessor common to all child classes
	NiVector3 position; // 02C - seems to be private
	float scale; // 038 

	TESObjectCELL* parentCell; // 03C
	ExtraDataList extraDataList; // 040
	RenderState* loadedData; // 05C

	ScriptEventList* GetEventList() const;

	bool IsTaken() const
	{
		return ((flags & kFlags_Taken) == kFlags_Taken);
	} // Need to implement
	bool IsPersistent() const
	{
		return ((flags & kFlags_Persistent) == kFlags_Persistent);
	}

	NiAVObject* GetNiBlock(const char* blockName) const;
	NiAVObject* GetNiBlock2(const char* blockName) const;
	NiNode* GetNode(const char* nodeName) const;
	NiNode* GetNode2(const char* nodeName) const;
	hkpRigidBody* GetRigidBody(const char* blockName) const;
	bhkCharacterController* GetCharacterController() const;
	void MoveToCell(TESObjectCELL* cell, const NiVector3& posVector);
	void SetPos(const NiVector3& posVector);
	bool CanHaveSound() const;
	void AttachSound(bool bAttach);
	__m128 __vectorcall GetTranslatedPos(const NiVector3& posMods) const;

	__forceinline NiNode* Get3DSimple() const
	{
		return loadedData ? loadedData->rootNode : nullptr;
	}
};

static_assert(offsetof(TESObjectREFR, baseForm) == 0x01C);
static_assert(sizeof(TESObjectREFR) == 0x60);
extern TESObjectREFR* s_tempPosMarker;

// 080
class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();
	virtual bool MoveToHigh();
	virtual bool MoveToLow();
	virtual bool MoveToMiddleLow();
	virtual bool MoveToMiddleHigh();
	virtual bhkCharacterController* Move(float afTimeDelta, NiPoint3& arDeltaMove, uint8_t abDefer);
	virtual bhkCharacterController* Jump();
	virtual void SetAvoidanceDisabled(bool abVal);
	virtual void Process(float afTimeDelta);
	virtual void ChangeProcessLevel();
	virtual void UpdateInDialogue(float afTime, DialogueResponse* apResponse, bool);
	virtual void UpdateAnimationAlt(float = 0.f);
	virtual bool GetBufferedPackageEvaluation();
	virtual void EvaluatePackage();
	virtual bool IsInCombat(bool abSearching);
	virtual void SetDialoguewithPlayer(bool abInDialog);
	virtual void InitiateDialogueFromPackage(TESPackage* apPackage);
	virtual bool InitiateDialogue(Actor* apTarget, void* apSubjectLocation, void* apTargetLocation, bool abHeadTrack, bool abMove,
	                              bool abInterateIndexFlag, TESTopic* apTopic, bool abTargetContinueMoving,
	                              bool abTargetMoveTowardStarter);
	virtual float SpeakSoundFunction(const char* apSoundFile, BSSoundHandle* apSoundHandle, uint32_t aeEmotionType,
	                                 uint32_t auiEmotionValue, uint32_t auiResponseLength, TESIdleForm* apSpeakerAnimation,
	                                 TESIdleForm* apListenerAnimation, Actor* apListener, bool abSetEmotion, bool ab2D,
	                                 bool abQueue, bool abLip, bool abForceSpeakerAnim);
	virtual void EndDialogue();
	virtual void SetRunOnceExtraPackage(TESPackage* apPackage, uint8_t aucDay);
	virtual bool HasEditorLocation();
	virtual TESWorldSpace* GetEditorLocationWorld();
	virtual TESObjectCELL* GetEditorLocationInteriorCell();
	virtual NiPoint3 GetEditorLocationCoord();
	virtual bool DetachCharController();
	virtual bool RemoveCharController();
	virtual void SetPosition(const NiPoint3& arPosition);
	virtual void GetLinearVelocity(NiPoint3& arVelocity);
	virtual void EvaluateDetection(float afDelta);
	virtual float GetDetectionTimer();
	virtual void ReduceDetectionTimer();
	virtual float GetHeading(bool abIgnoreImmobile);
	virtual void SetHeadingToRef(TESObjectREFR* apRef);
	virtual void SetHeading(float afValue);
	virtual TESObjectREFR* GetCurrentTarget();
	virtual bool IsAtPoint(NiPoint3& arPoint, float afRadius, bool abExpandRadius, bool abAlwaysTestHeight);
	virtual int32_t GetSpeakingEmotion();
	virtual void SetSpeakingEmotion(int32_t aiEmotion);
	virtual int32_t GetSpeakingEmotionValue();
	virtual void SetSpeakingEmotionValue(int32_t aiValue);
	virtual void SetSize(float afSize);
	virtual float GetAcrobatics();
	virtual bool IsDying();
	virtual bool IsTransient();
	virtual void SetDisplayGeometry(bool abShow);
	virtual void PutCreatedPackage(TESPackage* apPackage, bool abTempPackage, bool abIsaCreatedPackage);
	virtual void Update(float afDelta);
	virtual void CleanUpPointersOnDisable();
	virtual MobileObject* SetUpTalkingActivatorActor(MobileObject* apActor, MobileObject*&);

	BaseProcess* baseProcess; // 060
	UInt32 unk064[(0x080 - 0x064) >> 2]; // 064
	// 080
};

static_assert(sizeof(MobileObject) == 0x080);

// 00C
class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	virtual void CastAbility(SpellItem* apAbility, bool abLoadCast);
	virtual void CastAddiction(SpellItem* apAddiction, bool abLoadCast);
	virtual void CastRadiation(SpellItem* apRadiation, bool abLoadCast);
	virtual void CastSpellImmediate(MagicItem* apMagicItem, bool, MagicTarget* apTarget, float afEffectivenessMult,
	                                bool abAdjustOnlyHostileEffectiveness);
	virtual void TransferDisease(SpellItem* apDisease, MagicTarget* apTarget, bool a4);
	virtual void CastWornEnchantment(MagicItem* apSpell, TESBoundObject* apSource, bool abLoadCast);
	virtual MagicTarget* GetTouchTarget();
	virtual void StartAim();
	virtual void StartCast();
	virtual void SpellCast(MagicItem* apSpell, bool);
	virtual bool CheckCast(MagicItem* apSpell, float*, uint32_t* apReason, bool);
	virtual TESObjectREFR* GetCasterStatsObject();
	virtual NiNode* GetMagicNode();
	virtual MagicItem* GetCurrentSpell();
	virtual bool TargetSelf(ActiveEffect* apActiveEffect);
	virtual float GetMagicEffectivenessModifier(bool, float);
	virtual void SetCurrentSpell(MagicItem* apMagicItem);
	virtual MagicTarget* GetDesiredTarget();
	virtual void SetDesiredTarget(MagicTarget* apTarget);
	virtual ActiveEffect* CreateActiveEffect(MagicItem* apMagicItem, EffectItem* apEffectItem, TESBoundObject* apObject);

	enum State
	{
		NO_SPELL = 0,
		AIM = 1,
		CAST = 2,
		RELEASE = 3,
		FIND_TARGETS = 4,
		ERR_SPELL_DISABLED = 5,
		ERR_ALREADY_CASTING = 6,
		ERR_CANNOT_CAST = 7
	};

	void* pLight;
	State eState;
};

static_assert(sizeof(MagicCaster) == 0x00C);


class Actor : public MobileObject, public MagicCaster, public MagicTarget, public ActorValueOwner, public CachedValuesOwner
{
public:
	Actor();
	~Actor();
	virtual bool IsGuard();
	virtual void SetGuard(bool abVal);
	virtual void SetEquippedWeight(float afVal);
	virtual float GetEquippedWeight();
	virtual int32_t GetInfamy();
	virtual int32_t GetFame();
	virtual void SetIgnoreCrime(bool abVal);
	virtual bool GetIgnoreCrime();
	virtual void Resurrect(bool abResetInventory, bool abAttach3D, bool abGetupResurect);
	virtual void SetAdvanceNumber(uint32_t auiVal);
	virtual uint32_t GetAdvanceNumber();
	virtual void SetSkillAdvanceDelayed(UInt32 aeIndex);
	virtual UInt32 GetSkillAdvanceDelayed();
	virtual bool DoDamage(float afHealth, float afFatigue, Actor* apSource);
	virtual void UseActionPoints(float afAmount);
	virtual void UseActionPointsAlt(uint32_t auiAction);
	virtual uint32_t GetDisposition(Actor* apTarget, void* apOverride = nullptr);
	virtual void UpdateMovement(float afTimeDelta, bool);
	virtual void UpdateNonRenderSafeMovement(float afTimeDelta, bool);
	virtual void UpdateAnimationMovementNoWorldUpdate();
	virtual float GetTurningSpeed();
	virtual bool IsOverEncumbered();
	virtual bool IsPc();
	virtual bool GetCannibal();
	virtual void SetCannibal(bool abVal);
	virtual bool GetSandman();
	virtual void SetSandman(bool abVal);
	virtual void InitiateSandmanPackage(Actor* apTarget, TESObjectREFR* apFurnitureRef, FurnitureMark* apFurnitureMark,
	                                    uint8_t aucFurnitureMarkerIndex);
	virtual void InitiateCannibalPackage(Actor* apTarget);
	virtual TESRace* GetRace();
	virtual float GetReach();
	virtual void SetRefraction(bool abEnable, float afRefractionPower);
	virtual void SetHasRagdoll(bool abVal);
	virtual bool GetHasRagdoll();
	virtual UInt32 GetActorType();
	virtual void SetActorValueF(uint32_t auiAVCode, float afValue);
	virtual void SetActorValueI(uint32_t auiAVCode, int32_t aiValue);
	virtual void TempModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void TempModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void PermanentModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void PermanentModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void DamageModActorValueF(uint32_t auiAVCode, float afModifier, Actor* apAttacker);
	virtual void DamageModActorValueI(uint32_t auiAVCode, int32_t aiModifier, Actor* apAttacker);
	virtual void ModActorBaseValueF(uint32_t auiAVCode, float afModifier);
	virtual void ModActorBaseValueI(uint32_t auiAVCode, int32_t aiModifier);
	virtual void* GetBestWeapon(uint32_t aeWeaponType);
	virtual void* GetBestAmmo();
	virtual void ResetArmorRating();
	virtual bool DamageEquipment(void* apEquipment, float afDamage, bool abIgnoreArmorSkill = false);
	virtual TESObjectREFR* DropObject(TESForm* apItem, ExtraDataList* apExtraList, int32_t aiCount, const NiPoint3* apPoint,
	                                  const NiPoint3* apRotate);
	virtual void PickUpObject(TESObjectREFR* apObject, int32_t aiCount, bool abPlayPickUpSounds);
	virtual void CastScroll(TESObjectBOOK* apBook, MagicTarget* apTarget);
	virtual void CheckCastWhenStrikesEnchantment(void* apWeapon, Actor* apTarget, void* apArrow, bool& abInsufficientCharge);
	virtual void UpdateWornEnchantments(float afElapsedTime);
	virtual bool AddSpell(SpellItem* apSpell);
	virtual bool RemoveSpell(SpellItem* apSpell);
	virtual bool ReloadWeapon(TESObjectWEAP* apWeapon, uint32_t, bool);
	virtual void UseAmmo(uint32_t auiCount = UINT32_MAX);
	virtual void CheckTempModifiers();
	virtual void* GetCombatGroup();
	virtual void SetCombatGroup(void* apGroup);
	virtual bool InitiateTresPassPackage(void* apPackage);
	virtual void UpdateNonRenderSafeDialogueUpdate(float afDelta);
	virtual void InitiateSpectator();
	virtual void InitiateFlee(TESObjectREFR* apFleeRef, bool abRunOnce, bool abKnowsTarget, bool abCombatMode,
	                          TESObjectCELL* apCell, TESObjectREFR* apRef, float afFleeFromDist, float afFleeToDist);
	virtual void InitiateSearchForAttacker(TESObjectREFR* apAttacker, NiPoint3);
	virtual void InitiateGetUpPackage();
	virtual void SetAlpha(float afValue);
	virtual float GetAlpha();
	virtual void StartCombat(Actor* apTarget, void* apCombatGroup, bool abForceCombat, bool abIsAggressor, bool abFleeing,
	                         int32_t aiPriority, bool abCastSpell, TESPackage* apPackage);
	virtual void* GetCombatController();
	virtual Actor* GetCombatTarget();
	virtual void UpdateCombat(float afElapsedTime);
	virtual void StopCombat(Actor* apAttacker);
	virtual void PerformOneRoundofCombat();
	virtual float GetArmorRating();
	virtual float GetAttackDamage();
	virtual bool IsTrespassing();
	virtual void SetTrespassing();
	virtual bool GetUsesAttackPercents();
	virtual void GetPowerAttackPercent(uint32_t);
	virtual float CalculateWalkSpeed();
	virtual float CalculateRunSpeed();
	virtual void ModifyDispositionTowardActor(Actor* apActor, float afValue);
	virtual float GetDispositionModifierTowardActor(Actor* apActor);
	virtual void RemoveDispositionModifierForActor(Actor* apActor);
	virtual void SetActorStartingPosition();
	virtual bool HasBeenAttacked();
	virtual void SetBeenAttacked(bool abVal);
	virtual void HitMe(HitData* apHitData, float afHealthDmg, float afBlockDTMod, NiPoint3& unk, NiPoint3& arWeaponPos);
	virtual void UseSkill0(UInt32 aeSkill, uint32_t);
	virtual void UseSkill1(UInt32 aeSkill, uint32_t aeAction, float afModifier);
	virtual void UseSkill2(UInt32 aeSkill, float afUsage, TESSkill* apSkill);
	virtual void RewardExperience();
	virtual float GetBaseValueOverride(UInt32 aeIndex, bool& arFound);
	virtual bool SetBaseValueOverride(UInt32 aeIndex, float afValue);
	virtual void AddGunWobble();
	virtual void AddPerk(BGSPerk* apPerk, uint8_t aucRank, bool abTeammate);
	virtual void RemovePerk(BGSPerk* apPerk, bool abTeammate);
	virtual uint8_t GetPerkRank(BGSPerk* apPerk, bool abTeammate);
	virtual void AddPerkEntry(void* apPerkEntry, bool abTeammate);
	virtual void RemovePerkEntry(void* apPerkEntry, bool abTeammate);
	virtual void* GetPerkEntryList(uint8_t ucID, bool abTeammate);
	virtual void StartAnimOn1stPerson(uint16_t aeAnimGroup, UInt32 aeActionFlags);
	virtual bool IsImmobile();
	virtual void HandleHealthDamage(Actor* apAttacker, float afDamage);
	virtual void AttackedBy(Actor* apAttacker, ActiveEffect* apEffect = nullptr);
	virtual void HandleBlockedAttack(float afFullDamage, float afPercentageBlocked, Actor* apBlockingActor, void* apArrow);
	virtual void PrecacheData();
	virtual void ProcessTracking(float afTime);
	virtual void ProcessEmotions(float afTime);
	virtual void CreateActorMover();
	virtual void DestroyActorMover();
	virtual NiPoint3 CalculateAnticipatedLocation(float afTime);


	bhkRagdollController* ragDollController; // 0A4
	bhkRagdollPenetrationUtil* ragDollPentrationUtil; // 0A8
	UInt32 unk0AC[(0xB8 - 0x0AC) >> 2]; // 0AC
	Actor* killer; // 0B8
	UInt32 unk0BC[(0xF8 - 0x0BC) >> 2]; // 0BC
	bool inCombat; // 0F8
	UInt32 lifeState; // 0FC
	UInt32 unk100[(0x138 - 0x0100) >> 2]; // 100
	bool ignoreCrime; // 138
	UInt8 byte139; // 139
	UInt8 byte13A; // 13A
	UInt8 byte13B; // 13B
	UInt32 unk13C; // 13C
	UInt8 inWater; // 140
	UInt8 isSwimming; // 141
	UInt16 unk142; // 142
	UInt32 unk144[(0x184 - 0x144) >> 2]; // 144
	void* actorMover; // 184
	UInt32 unk188[(0x19C - 0x188) >> 2]; // 188
	UInt32 sitSleepState;

	bool IsEssential()
	{
		return ThisCall<bool>(0x6F4A10, this);
	}

	float GetRadiationLevel(bool scaleByResist);
	TESActorBase* GetActorBase();

	SInt8 GetKnockedState()
	{
		return (baseProcess && (baseProcess->uiProcessLevel <= 1))
			       ? static_cast<MiddleHighProcess*>(baseProcess)->eKnockedState
			       : -1;
	}

	AnimData* GetAnimation() const
	{
		if (this->baseProcess) return this->baseProcess->GetAnimation();
		return nullptr;
	}
};

static_assert(sizeof(Actor) == 0x1A0);

class BipedAnim
{
public:
	enum eOptionalBoneType
	{
		kOptionalBone_Bip01Head = 0,
		kOptionalBone_Weapon = 1,
		kOptionalBone_Bip01LForeTwist = 2,
		kOptionalBone_Bip01Spine2 = 3,
		kOptionalBone_Bip01Neck1 = 4,
	};

	// 008
	struct OptionalBone
	{
		bool bExists;
		NiNode* pBone;
	};

	// 010
	struct VB01Data
	{
		union
		{
			TESForm* pItem;
			TESObjectARMO* pArmor;
			TESObjectWEAP* pWeapon;
			TESRace* pRace;
		};

		TESModelTextureSwap* pModelTexture;
		NiNode* pBoneNode;
		UInt32 unk00C;
	};

	NiNode* pRoot;
	OptionalBone kBones[5];
	VB01Data slotData[20];
	VB01Data unk016C[20];
	UInt32 unk2AC;
	Actor* pActor;
};

static_assert(sizeof(BipedAnim) == 0x2B4);

class Character : public Actor
{
public:
	Character();
	~Character();

	BipedAnim* bipedAnim;
	float fTotalArmorDR;
	UInt8 bIsTrespassing;
	UInt8 bIsGuard;
	UInt16 unk1C2;
	float fFlyInventoryWeight;
};

static_assert(sizeof(Character) == 0x1B0);

class Creature : public Actor
{
public:
	Creature();
	~Creature();
	bool bHasRagdoll;
	float fTotalArmorDR;
};

static_assert(sizeof(Creature) == 0x1A8);


// 9BC

struct CompassTarget
{
	Actor* target;
	UInt8 isHostile;
	UInt8 isUndetected;
};

static_assert(sizeof(CompassTarget) == 8);


class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	// used to flag controls as disabled in disabledControlFlags
	enum
	{
		kControlFlag_Movement = 1 << 0,
		kControlFlag_Look = 1 << 1,
		kControlFlag_Pipboy = 1 << 2,
		kControlFlag_Fight = 1 << 3,
		kControlFlag_POVSwitch = 1 << 4,
		kControlFlag_RolloverText = 1 << 5,
		kControlFlag_Sneak = 1 << 6,
	};

	struct WobbleNodes
	{
		NiNode* wobbleAnimNodes[12];
		NiNode* wobbleAnimNodes2[12];
	};

	// lotsa data
	UInt32 unk188[(0x570 - 0x1B0) >> 2]; // 188
	BGSNote* note; // 570
	UInt32 unk574; // 574
	ImageSpaceModifierInstanceDOF* unk578; // 578
	ImageSpaceModifierInstanceDOF* unk57C; // 57C
	ImageSpaceModifierInstanceDRB* unk580; // 580
	UInt32 unk584[(0x5A8 - 0x584) >> 2]; // 584
	bool bIs3rdPerson;
	bool bThirdPerson; // 5A9
	UInt8 unk5A9[2];
	UInt32 unk5AC[(0x5DC - 0x5AC) >> 2]; // 5AC
	UInt32 disabledControlFlags; // 5DC
	UInt32 unk5E0; // 5E0
	UInt32 unk5E4; // 5E4
	BipedAnim* pBipedAnims1st; // 5E8
	AnimData* firstPersonAnimData; // 5EC
	NiNode* node1stPerson; // 5F0
	float eyeHeight; // 5F4
	void* spInventoryMenu;
	AnimData* pInventoryAnimation;
	void* pInventoryWeaponEffect;
	TESTopic* topic; // 604
	UInt32 unk608[3]; // 608
	TESQuest* quest; // 614
	BGSQuestObjective* questObjective; // 618
	UInt32 unk61C[(0x68C - 0x61C) >> 2]; // 61C
	float timeGrenadeHeld; // 68C
	UInt8 unk690[(0x704 - 0x690)]; // 690
	UInt32 gameDifficulty; // 704
	UInt8 unk708[(0x718 - 0x708)]; // 708
	float flycamZRot; // 718
	float flycamXRot; // 71C
	float flycamPosX; // 720
	float flycamPosY; // 724
	float flycamPosZ; // 728
	UInt8 unk72C[440];
	Actor* autoAimActor;
	float kBulletAutoAim[3];
	UInt32 unk8F4;
	bool bIsActorWithinPickDistance;
	Actor* reticleActor;
	tList<CompassTarget>* compassTargets;
	float fPipboyLightHeldTime;
	bool bShouldOpenPipboy;
	char byteD55;
	char byteD56;
	char byteD57;
	NiPoint3 camera3rdPos;
	void* pCombatGroup;
	int unk91C;
	int iTeammateCount;
	float fCombatYieldRetryTimer;
	WobbleNodes wobbleAnims;
	NiVector3 kCamera1stPos;
	NiVector3 kCameraPos;
	void* spRigidBody;
	bool pcInCombat; // 9A4
	bool pcUnseen; // 9A5
	UInt8 unk9A6[(0x9BC - 0x9A6)]; // 9A6

	static PlayerCharacter* GetSingleton();
	AnimData* GetAnimData(bool bFirstPerson) const;
	AnimData* GetAnimData() const;
};

static_assert(sizeof(PlayerCharacter) == 0x9BC);
static_assert(offsetof(PlayerCharacter, fCombatYieldRetryTimer) == 0x924);
static_assert(offsetof(PlayerCharacter, bThirdPerson) == 0x5A9);
static_assert(offsetof(PlayerCharacter, disabledControlFlags) == 0x5DC);
static_assert(offsetof(PlayerCharacter, camera3rdPos) == 0x90C);

struct VATSCameraData
{
	tList<void> targetsList; // 00
	UInt32 mode; // 08
	UInt32 unk0C; // 0C
	BGSCameraShot* camShot; // 10
	float flt14; // 14
	float flt18; // 18
	UInt32 unk1C; // 1C
	UInt32 unk20; // 20
	UInt32 unk24; // 24
	void* isModInstForm; // 28
	void* isModInstRB; // 2C
	UInt32 unk30; // 30
	void* object34; // 34
	UInt8 byte38; // 38
	UInt8 pad39[3]; // 39
	UInt32 unk3C; // 3C
	UInt32 unk40; // 40
	UInt32 unk44; // 44
	void Free()
	{
		ThisCall(0x7CF2A0, this);
	}

	static VATSCameraData* GetSingleton()
	{
		return (VATSCameraData*)0x108D098;
	};
};

static_assert(sizeof(VATSCameraData) == 0x48);

// 18
class LoadedReferenceMap : public NiTPointerMap<TESObjectREFR>
{
public:
	UInt32 unk10; // 10
	UInt32 unk14; // 14
};

// C0
struct LoadedReferenceCollection
{
	LoadedReferenceMap map00; // 00	Unused?
	LoadedReferenceMap allInventoryObjects; // 18
	LoadedReferenceMap weaponObjects; // 30
	LoadedReferenceMap ammoObjects; // 48
	LoadedReferenceMap containers; // 60
	LoadedReferenceMap actors; // 78
	LoadedReferenceMap radiationEmitters; // 90
	LoadedReferenceMap teleportDoors; // A8

	__forceinline static LoadedReferenceCollection* Get()
	{
		return *(LoadedReferenceCollection**)0x1070B34;
	}
};

class BSGameSound
{
public:
	/*00*/
	virtual BSGameSound* Destroy(bool doFree);
	/*04*/
	virtual bool CheckStateFlagsBit5();
	/*08*/
	virtual bool CheckStateFlagsBit19();
	/*0C*/
	virtual bool CheckStateFlagsBit6();
	/*10*/
	virtual float GetVolume();
	/*14*/
	virtual void SetIsLooping(bool doSet);
	/*18*/
	virtual void Unk_06(void);
	/*1C*/
	virtual void Unk_07(void);
	/*20*/
	virtual void Unk_08(void);
	/*24*/
	virtual void Unk_09(void);
	/*28*/
	virtual void Unk_0A(void);
	/*2C*/
	virtual void CopyFrom(BSGameSound* source, bool arg2);
	/*30*/
	virtual void Unk_0C(void);
	/*34*/
	virtual bool Unk_0D(void);
	/*38*/
	virtual bool Unk_0E(void);
	/*3C*/
	virtual bool SetVolume(float inVol);
	/*40*/
	virtual void Unk_10(void);
	/*44*/
	virtual bool Unk_11(void);
	/*48*/
	virtual void Unk_12(void);
	/*4C*/
	virtual void Unk_13(float arg1, float arg2, float arg3);
	/*50*/
	virtual void Unk_14(float arg1, float arg2, float arg3);
	/*54*/
	virtual void Unk_15(void* arg1);
	/*58*/
	virtual void Unk_16(void);
	/*5C*/
	virtual void Unk_17(float arg1, float arg2);
	/*60*/
	virtual void Unk_18(UInt16 arg1, UInt16 arg2, UInt16 arg3, UInt16 arg4, UInt16 arg5);
	/*64*/
	virtual bool SetFrameFrequencyPerc(float freqPerc);
	/*68*/
	virtual float GetFrameFrequencyPerc();
	/*6C*/
	virtual void Seek(UInt32 timePoint);

	enum SoundFlags
	{
		kFlag_2D = 1,
		kFlag_3D = 2,
		kFlag_IsVoice = 4,
		kFlag_IsFootsteps = 8,
		kFlag_Loop = 0x10,
		kFlag_NotDialogue = 0x20,
		kFlag_RandomFrequencyShift = 0x40,
		kFlag_UnkBit7 = 0x80,
		kFlag_UnkBit8 = 0x100,
		kFlag_UnkBit9 = 0x200,
		kFlag_UnkBit10 = 0x400,
		kFlag_IsMusic = 0x800,
		kFlag_MuteWhenSubmerged1 = 0x1000,
		kFlag_MaybeUnderwater = 0x2000,
		kFlag_DonotCache = 0x10000,
		kFlag_FirstPerson = 0x40000,
		kFlag_Modulated = 0x80000,
		kFlag_IsRadio = 0x100000,
		kFlag_UnkBit22 = 0x400000,
		kFlag_EnvelopeFast = 0x2000000,
		kFlag_EnvelopeSlow = 0x4000000,
		kFlag_2DRadius = 0x8000000
	};

	enum Flags00C
	{
		kFlag_MuteWhenSubmerged = 1,
		kFlag_StartAtRandomPosition = 2,
	};

	enum StateFlags
	{
		kState_Stopped = 0x10,
		kState_UnkBit5 = 0x20,
		kState_UnkBit6 = 0x40,
		kState_UnkBit9 = 0x200,
		kState_Muted = 0x400,
		kState_UnkBit15 = 0x8000,
		kState_UnkBit16 = 0x10000,
		kState_IsLooping = 0x20000,
		kState_UnkBit19 = 0x80000,
		kState_UnkBit20 = 0x100000,
	};

	UInt32 mapKey; // 004
	UInt32 soundFlags; // 008
	UInt32 flags00C; // 00C
	UInt32 stateFlags; // 010
	UInt32 duration; // 014
	UInt16 staticAttenuation; // 018	dB * -1000
	UInt16 unk01A; // 01A
	UInt16 unk01C; // 01C
	UInt16 unk01E; // 01E
	UInt16 unk020; // 020
	UInt16 unk022; // 022
	float volume; // 024	0.0 to 1.0
	float flt028; // 028
	float flt02C; // 02C
	UInt32 unk030; // 030
	UInt16 baseSamplingFreq; // 034
	char filePath[254]; // 036	Originally: filePath[260]
	TESSound* sourceSound; // 134	"Stolen" from filePath
	float frequencyMod; // 138	^
	float maxAttenuationDist; // 13C
	float minAttenuationDist; // 140
	UInt32 pathHashFile; // 144
	UInt32 pathHashFolder; // 148
	UInt32 unk14C; // 14C
	float flt150; // 150
	UInt32 unk154; // 154
	SInt8 randFrequencyShift; // 158
	UInt8 byte159; // 159
	UInt16 samplingFreq; // 15A
	UInt32 unk15C; // 15C
	UInt32 unk160; // 160
	UInt32 unk164; // 164
	UInt32 unk168; // 168
	UInt32 unk16C; // 16C
	UInt32 unk170; // 170
	UInt32 unk174[5]; // 174
	UInt32 priority; // 188
	UInt32 unk18C[3]; // 18C
};

static_assert(sizeof(BSGameSound) == 0x198);

class BSAudioManager
{
public:
	virtual void Destroy(bool doFree);

	// let's hope these are the same
	enum AudioFlags : UInt32
	{
		kAudioFlags_2D = 0x1,
		kAudioFlags_3D = 0x2,
		kAudioFlags_IsVoice = 0x4,
		kAudioFlags_IsFootsteps = 0x8,
		kAudioFlags_Loop = 0x10,
		kAudioFlags_SystemSound = 0x20,
		kAudioFlags_RandomFrequencyShift = 0x40,
		kAudioFlags_80 = 0x80,
		kAudioFlags_100 = 0x100,
		kAudioFlags_IsMusic = 0x800,
		kAudioFlags_RegionSound_MuteWhenSubmerged = 0x1000,
		kAudioFlags_MaybeUnderwater = 0x2000,
		kAudioFlags_Impact = 0x4000,
		kAudioFlags_Cached = 0x8000,
		kAudioFlags_DontCache = 0x10000,
		kAudioFlags_20000 = 0x20000,
		kAudioFlags_FirstPerson = 0x40000,
		kAudioFlags_Modulated = 0x80000,
		kAudioFlags_IsRadio = 0x100000,
		kAudioFlags_IgnoreTimescale = 0x200000,
		kAudioFlags_Radio400000 = 0x400000,
		kAudioFlags_IsMusic2 = 0x800000,
		kAudioFlags_1000000 = 0x1000000,
		kAudioFlags_EnvelopeFast = 0x2000000,
		kAudioFlags_EnvelopeSlow = 0x4000000,
		kAudioFlags_2DRadius = 0x8000000,
		kAudioFlags_BeamEmitter = 0x10000000,
		kAudioFlags_20000000 = 0x20000000,
		kAudioFlags_AnimationDriven = 0x40000000,
	};

	// 10
	struct AudioRequest
	{
		UInt32 count; // 00
		UInt8 byte04; // 04
		UInt8 pad05[3]; // 05
		void* requestData; // 08
		UInt32 tickCount; // 0C
	};

	UInt32 unk004[8]; // 004
	AudioRequest request024; // 024
	AudioRequest request034; // 034
	AudioRequest request044; // 044
	NiTPointerMap<BSGameSound> playingSounds; // 054
	NiTPointerMap<void> playingSoundInfos1; // 064
	NiTPointerMap<void> playingSoundInfos2; // 074
	NiTPointerMap<NiAVObject> soundPlayingObjects; // 084
	DList<BSGameSound> cachedSounds; // 094
	UInt32 unk0A0; // 0A0
	UInt32 unk0A4; // 0A4
	float flt0A8; // 0A8
	CRITICAL_SECTION cs0AC;
	CRITICAL_SECTION cs0C4;
	CRITICAL_SECTION cs0DC;
	CRITICAL_SECTION cs0F4;
	UInt32 lastTickCount;
	BYTE initedSounds;
	BYTE unk111;
	UInt32 threadID;
	void* updateThread;
	float volumes[6];
	UInt32 unk134;
	UInt8 byte138;
	UInt8 byte139;
	UInt32 unk13C;
	UInt32 unk140;
	UInt32 soundCacheIndex;

	__forceinline static BSAudioManager* Get()
	{
		return (BSAudioManager*)0x11791D8;
	}
};

static_assert(sizeof(BSAudioManager) == 0x148);


// 04
class AnimSequenceBase
{
public:
	virtual void Destroy(bool deFree);
	virtual void AddAnimGroupSequence(BSAnimGroupSequence* sequence, UInt32 arg2);
	virtual bool RemoveAnimGroupSequence(BSAnimGroupSequence* sequence, UInt32 arg2);
	virtual bool IsSingle();
	virtual BSAnimGroupSequence* GetSequenceByIndex(UInt8 index);
	virtual BSAnimGroupSequence* GetSequenceByGroup(TESAnimGroup* animGroup);
	virtual char GetSequenceIndexByName(const char* seqName);
};

struct KFModel;

// 38
class AnimIdle : public NiRefObject
{
public:
	UInt32 unk08; // 08
	UInt32 unk0C; // 0C
	UInt32 unk10; // 10
	UInt32 sequenceID; // 14
	BSAnimGroupSequence* agSequence; // 18
	NiObject* object1C; // 1C
	NiObject* object20; // 20
	NiObject* object24; // 24
	NiObject* object28; // 28
	TESIdleForm* idleForm; // 2C
	UInt32 unk30; // 30
	Actor* actor; // 34
};

// 13C
struct AnimData
{
	enum SequenceTypes
	{
		kSequence_None = -1,
		kSequence_Idle = 0,
		kSequence_Movement = 1,
		kSequence_LeftArm = 2,
		kSequence_LeftHand = 3,
		kSequence_Weapon = 4,
		kSequence_WeaponUp = 5,
		kSequence_WeaponDown = 6,
		kSequence_SpecialIdle = 7,
		kSequence_Death = 0x14
	};

	enum AnimStates
	{
	};

	struct PlayingIdle
	{
		TESIdleForm* idleForm;
		float replayDelay;
	};

	UInt32 unk000; // 000
	Actor* actor; // 004
	NiNode* nSceneRoot; // 008
	NiNode* nBip01; // 00C
	UInt32 unk010; // 010
	float flt014; // 014
	float flt018; // 018
	NiVector3 pos01C; // 01C
	NiNode* nPelvis; // 028
	NiNode* nBip01Copy; // 02C
	NiNode* nLForearm; // 030
	NiNode* nHead; // 034
	NiNode* nWeapon; // 038
	UInt32 unk03C[2]; // 03C
	NiNode* nNeck1; // 044
	float flt048; // 048
	UInt16 animGroupIDs[8]; // 04C
	SInt32 sequenceState1[8]; // 05C
	SInt32 sequenceState2[8]; // 07C
	UInt16 word09C[8]; // 09C
	UInt32 unk0AC[8]; // 0AC
	UInt8 byte0CC; // 0CC
	UInt8 byte0CD; // 0CD
	UInt8 byte0CE; // 0CE
	UInt8 byte0CF; // 0CF
	float timePassed; // 0D0
	UInt32 unk0D4; // 0D4
	NiControllerManager* controllerMngr; // 0D8
	NiTPointerMap<AnimSequenceBase>* sequenceBaseMap; // 0DC
	BSAnimGroupSequence* animSequence[8]; // 0E0
	BSAnimGroupSequence* animSeq100; // 100
	tList<KFModel> loadingAnims; // 104
	float movementSpeedMult; // 10C
	float rateOfFire; // 110
	UInt8 noBlend; // 120
	UInt8 byte121; // 121
	UInt16 word122; // 122
	AnimIdle* idleAnim; // 124
	AnimIdle* queuedIdleAnim; // 128
	NiObject* object12C; // 12C
	NiObject* object130; // 130
	tList<PlayingIdle> playingIdleAnims; // 134

	TESIdleForm* GetPlayedIdle() const;

	__forceinline void PlayIdle(TESIdleForm* idleAnim)
	{
		ThisCall(0x4649F0, this, idleAnim, actor, idleAnim->data.groupFlags & 0x3F, 3);
	}

	__forceinline void StopIdle()
	{
		ThisCall(0x460090, this, true, false);
	}

	__forceinline void Refresh()
	{
		ThisCall(0x45E360, this, 0);
	}

	void BlendSequence(UInt32 sequenceIdx)
	{
		if (animSequence[sequenceIdx]) ThisCall(0x45E4C0, this, sequenceIdx, 0);
	}
};

static_assert(sizeof(AnimData) == 0x130);

class BSTreeManager
{
public:
	virtual void Destructor();

	NiRefObject* unk04;
	void* materialProperty08;
	void* alphaProperty0C;
	void* bsxFlags;
	float unk14;
	UInt8 byte18;
	UInt8 byte19;
	UInt8 byte1A;
	UInt8 isForceFullLOD;
	UInt32 refTreeNodeMap;

	static BSTreeManager* GetSingleton(bool bEnableCreation)
	{
		return CdeclCall<BSTreeManager*>(0x59BAA0, bEnableCreation);
	}
};

static_assert(sizeof(BSTreeManager) == 0x20);

struct SkySound
{
	BSSoundHandle sound;
	TESWeather* pWeather;
	UInt32 eSoundType;
	UInt32 id;
	UInt32 uiData;
};

class SkyObject
{
public:
	virtual ~SkyObject();
	virtual NiNode* GetRoot() const;
	virtual void Initialize(NiNode* apRoot);
	virtual void Update(Sky* apSky, float afValue);

	NiPointer<NiNode> spRoot;
};

static_assert(sizeof(SkyObject) == 0x8);

class Clouds : public SkyObject
{
public:
	DWORD spClouds[4];
	DWORD spTransTexture[4];
	NiColor pColors[4];
	UInt16 usNumLayers;
	bool bForceTransTextureUpdate;

	void ForceTransTextureUpdate();
};

static_assert(sizeof(Clouds) == 0x5C);

class Sky
{
public:
	virtual Sky* Destructor(bool doFree);

	enum Mode : UInt32
	{
		SM_NONE = 0x0,
		SM_INTERIOR = 0x1,
		SM_SKYDOME_ONLY = 0x2,
		SM_FULL = 0x3,
	};

	enum Flags : UInt32
	{
		REFRESH_WEATHER = 0x1,
		FORCED_UPDATE = 0x2,
		UNDERWATER = 0x4,
		REFRESH_ACCELERATION = 0x8,
		FAST_TRAVEL = 0x10,
		REFRESH_MOON = 0x20,
		REFRESH_CLIMATE = 0x40,
		HIDE_SKY = 0x80,
		REFRESH_SUNRISE_BEGIN = 0x100,
		REFRESH_SUNRISE_END = 0x200,
		REFRESH_SUNSET_BEGIN = 0x400,
		REFRESH_SUNSET_END = 0x800,
		REFRESH_SUNRISE_COLOR = 0x1000,
		REFRESH_SUNSET_COLOR = 0x2000,
	};


	void* spRoot;
	NiNode* spMoonsRoot;
	TESClimate* pCurrentClimate;
	TESWeather* pCurrentWeather;
	TESWeather* pLastWeather;
	TESWeather* pDefaultWeather;
	TESWeather* pOverrideWeather;
	void* pAtmosphere;
	void* pStars;
	void* pSun;
	Clouds* pClouds;
	void* pMasser;
	void* pSecunda;
	void* pPrecipitation;
	NiColor kColors[10];
	NiColor kWaterFogColor;
	NiColor kColorSunFog;
	float fWindSpeed;
	float fWindAngle;
	float fFogNearPlane;
	float fFogFarPlane;
	UInt32 unk0DC;
	UInt32 unk0E0;
	UInt32 fFogHeight;
	float fFogPower;
	float fCurrentGameHour;
	float fLastWeatherUpdate;
	float fCurrentWeatherPct;
	Mode eMode;
	tList<SkySound>* pSkySoundList;
	float fFlash;
	UInt32 uiFlashTime;
	UInt32 uiLastMoonPhaseUpdate;
	float fWindowReflectionTimer;
	float fAccelBeginPct;
	UInt32 unk114;
	Flags uiFlags;
	void* pFadeInIMODCurrent;
	void* pFadeOutIMODCurrent;
	void* pFadeInIMODLast;
	void* pFadeOutIMODLast;
	float f12_0;
	float f23_99;
	float f0_0;

	__forceinline static Sky* Get()
	{
		return *(Sky**)0x107A0E8;
	}

	__forceinline void RefreshClimate(TESClimate* climate, bool immediate = true)
	{
		ThisCall(0x57E3B0, this, climate, immediate);
	}

	__forceinline bool GetIsRaining()
	{
		return ThisCall(0x50EA20, this);
	}
};

static_assert(sizeof(Sky) == 0x138);
static_assert(offsetof(Sky, fFlash) == 0x100);
static_assert(offsetof(Sky, fCurrentGameHour) == 0xEC);
static_assert(offsetof(Sky, pCurrentWeather) == 0x10);

class CAsyncStream
{
public:
	CAsyncStream();
	~CAsyncStream();

	virtual void* Destroy(bool doFree);
	virtual void Seek();
	virtual void Read();
	virtual void GetFileSize();
	virtual void Unk04();
	virtual void Lock();
	virtual void Unlock();
};

class CBSAStream : public CAsyncStream
{
public:
	CBSAStream();
	~CBSAStream();
	char streamPath[260];
	BSFile* pFile;
	struct _RTL_CRITICAL_SECTION kLock;
	uint32_t unk124;
	LARGE_INTEGER ullFileSize;
	LARGE_INTEGER ullBytesRead;
};

static_assert(sizeof(CBSAStream) == 0x138);

struct RadioEntry
{
	TESObjectREFR* pReference;
	// TODO
};


class Date
{
public:
	UInt16 usDate;
	UInt16 usYear;
};

class Calendar
{
public:
	enum Season : int
	{
		NONE = -1,
		SPRING = 0,
		SUMMER = 1,
		FALL = 2,
		WINTER = 3,
		COUNT = 4,
	};

	TESGlobal* pGameYear;
	TESGlobal* pGameMonth;
	TESGlobal* pGameDay;
	TESGlobal* pGameHour;
	TESGlobal* pGameDaysPassed;
	TESGlobal* pTimeScale;
	UInt32 uiMidnightsPassed;
	bool bGameLoaded;

	static Calendar* GetSingleton();

	UInt32 GetYear() const;
	UInt32 GetMonth() const;
	UInt32 GetDay() const;
	float GetHour() const;
	float GetMinutesPassed() const;
	UInt32 GetHoursPassed() const;
	float GetGameDaysPassed() const;
	Season GetSeason() const;
	float GetTimeScale() const;
};

static_assert(sizeof(Date) == 0x4);
static_assert(sizeof(Calendar) == 0x20);


class HitData
{
public:
	HitData();
	~HitData();

	Actor* pSource;
	Actor* pTarget;
	void* projectileOrExplosion;
	UInt32 eWeaponSkill;
	UInt32 eDamageLimb;
	float fHealthDamage;
	float fBaseWeaponDamage;
	float fFatigueDamage;
	float fLimbDamage;
	float fBlockDTMod;
	float fArmorDamage;
	float fDamageToWeapon;
	TESObjectWEAP* pWeapon;
	float fHealthPercent;
	NiPoint3 kImpactPos;
	NiPoint3 kImpactAngle;
	SpellItem* pCriticalEffect;
	void* pVATSAction;
	UInt32 uiFlags;
	float fDamageMult;
	uint32_t uiRefCount;
};

static_assert(sizeof(HitData) == 0x64);
