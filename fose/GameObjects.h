#pragma once

#include "GameForms.h"
#include "GameBSExtraData.h"
#include "netimmerse.h"
#include "havok.h"
#if RUNTIME


static const UInt32 kTESObjectREFR_IsOffLimitsToPlayerAddr = 0x004DEBF0;


#else

static const UInt32 kTESObjectREFR_IsOffLimitsToPlayerAddr = 0;

#endif

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
	/*000*/virtual bool		ApplyEffect(MagicCaster* magicCaster, MagicItem* magicItem, ActiveEffect* activeEffect, bool arg4);
	/*004*/virtual Actor* GetActor();
	/*008*/virtual tList<ActiveEffect>* GetEffectList();
	/*00C*/virtual bool		Unk_03();
	/*010*/virtual bool		CannotBeHit();
	/*014*/virtual void		Unk_05(ActiveEffect* activeEffect);
	/*018*/virtual void		Unk_06(ActiveEffect* activeEffect);
	/*01C*/virtual void		Unk_07(MagicCaster* magicCaster, ActiveEffect* activeEffect);
	/*020*/virtual void		Unk_08(MagicCaster* magicCaster, ActiveEffect* activeEffect);
	/*024*/virtual float	GetEffectMagnitudeModifier(MagicCaster* magicCaster, MagicItem* magicItem, ActiveEffect* activeEffect);
	/*028*/virtual void		Unk_0A(MagicCaster* magicCaster, MagicItem* magicItem, ActiveEffect* activeEffect, bool arg4);
	/*02C*/virtual bool		Unk_0B(MagicCaster* magicCaster, MagicItem* magicItem, ActiveEffect* activeEffect);

	struct SpellInfo
	{
		MagicItem* magItem;
		MagicCaster* magCaster;
		ActiveEffect* activeEff;
	};

	UInt8				byte04;		// 04
	UInt8				byte05;		// 05
	UInt8				pad06[2];	// 06
	tList<SpellInfo>	spellInfos;	// 08

	void RemoveEffect(EffectItem* effItem);
};
static_assert(sizeof(MagicTarget) == 0x10);

class TESPackageData
{
public:
	TESPackageData();
	~TESPackageData();
	virtual void	Destroy(bool free);
	virtual void	CopyFrom(TESPackageData* packageData);
	virtual void	Unk_02(void);
	virtual void	Save(ModInfo* modInfo);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
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
	TESPackage* package;		// 00
	TESPackageData* packageData;	// 04
	TESObjectREFR* targetRef;		// 08
	UInt32			unk0C;			// 0C	Initialized to 0FFFFFFFFh, set to 0 on start
	float			unk10;			// 10	Initialized to -1.0	. Set to GameHour on start so some time
	UInt32			flags;			// 14	Flags, bit0 would be not created and initialized
};
class bhkCharacterController;
class BaseProcess
{
public:
	BaseProcess();
	~BaseProcess();

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
			kCached_Fatigue = 0x40000000,
			kCached_SurvivalSkillMult = 0x80000000,
		};
		float radius;
		float widthX;
		float widthY;
		float height;
		float DPS;
		float medicineSkillMult;
		float survivalSkillMult;
		float paralysis;
		float healRate;
		float fatigueReturnRate;
		float perceptionCondition;
		float eyeHeight;
		int SetAggression;
		int unk34;
		float walkSpeed;
		float runSpeedMult;
		unsigned __int8 hasNoCrippledLegs;
		unsigned __int8 pad41[3];
		unsigned int flags;
	};

	virtual void	Destroy(bool noDealloc);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);
	virtual void	Unk_17(void);
	virtual void	Unk_18(void);
	virtual void	Unk_19(void);
	virtual void	Unk_1A(void);
	virtual void	Unk_1B(void);
	virtual void	Unk_1C(void);
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(void);
	virtual void	Unk_1F(void);
	virtual void	Unk_20(void);
	virtual void	Unk_21(void);
	virtual void	Unk_22(void);
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual void	Unk_4A(void);
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);
	virtual void* GetWeaponInfo();
	virtual void* GetAmmoInfo();
	virtual void	Unk_54(void);
	virtual void	Unk_55(void);
	virtual void	Unk_56(void);
	virtual void	Unk_57(void);
	virtual void	Unk_58(void);
	virtual void	Unk_59(void);
	virtual void	Unk_5A(void);
	virtual void	Unk_5B(void);
	virtual void	Unk_5C(void);
	virtual void	Unk_5D(void);
	virtual void	Unk_5E(void);
	virtual void	Unk_5F(void);
	virtual void	Unk_60(void);
	virtual void	Unk_61(void);
	virtual void	Unk_62(void);
	virtual void	Unk_63(void);
	virtual void	Unk_64(void);
	virtual void	Unk_65(void);
	virtual void	Unk_66(void);
	virtual void	Unk_67(void);
	virtual void	Unk_68(void);
	virtual void	Unk_69(void);
	virtual void	Unk_6A(void);
	virtual void	Unk_6B(void);
	virtual void	Unk_6C(void);
	virtual void	Unk_6D(void);
	virtual void	Unk_6E(void);
	virtual void	Unk_6F(void);
	virtual void	Unk_70(void);
	virtual void	Unk_71(void);
	virtual void	Unk_72(void);
	virtual void	Unk_73(void);
	virtual void	Unk_74(void);
	virtual void	Unk_75(void);
	virtual void	Unk_76(void);
	virtual void	Unk_77(void);
	virtual void	Unk_78(void);
	virtual void	Unk_79(void);
	virtual void	Unk_7A(void);
	virtual void	Unk_7B(void);
	virtual void	Unk_7C(void);
	virtual void	Unk_7D(void);
	virtual void	Unk_7E(void);
	virtual void	Unk_7F(void);
	virtual void	Unk_80(void);
	virtual void	Unk_81(void);
	virtual void	Unk_82(void);
	virtual void	Unk_83(void);
	virtual void	Unk_84(void);
	virtual void	Unk_85(void);
	virtual void	Unk_86(void);
	virtual void	Unk_87(void);
	virtual void	Unk_88(void);
	virtual void	Unk_89(void);
	virtual void	Unk_8A(void);
	virtual void	Unk_8B(void);
	virtual void	Unk_8C(void);
	virtual void	Unk_8D(void);
	virtual void	Unk_8E(void);
	virtual void	Unk_8F(void);
	virtual void	Unk_90(void);
	virtual void	Unk_91(void);
	virtual void	Unk_92(void);
	virtual void	Unk_93(void);
	virtual void	Unk_94(void);
	virtual void	Unk_95(void);
	virtual void	Unk_96(void);
	virtual void	Unk_97(void);
	virtual void	Unk_98(void);
	virtual void	Unk_99(void);
	virtual void	Unk_9A(void);
	virtual void	Unk_9B(void);
	virtual void	Unk_9C(void);
	virtual PackageInfo*	GetRunningPackage();
	virtual void	Unk_9E(void);
	virtual void	Unk_9F(void);
	virtual void	Unk_A0(void);
	virtual void	Unk_A1();
	virtual bhkCharacterController*	GetCharacterController();
	virtual void	Unk_A3();
	virtual void	Unk_A4();
	virtual void	Unk_A5();
	virtual void	Unk_A6();
	virtual void	Unk_A7();
	virtual void	Unk_A8();
	virtual void	Unk_A9();
	virtual void	Unk_AA();
	virtual void	Unk_AB();
	virtual void	Unk_AC();
	virtual void	Unk_AD();
	virtual void	Unk_AE();
	virtual void	Unk_AF();
	virtual void	Unk_B0();
	virtual void	Unk_B1();
	virtual void	Unk_B2();
	virtual void	Unk_B3();
	virtual void	Unk_B4();
	virtual void	Unk_B5();
	virtual void	Unk_B6();
	virtual void	Unk_B7();
	virtual void	Unk_B8();
	virtual void	Unk_B9();
	virtual void	Unk_BA();
	virtual void	Unk_BB();
	virtual void	Unk_BC();
	virtual void	Unk_BD();
	virtual void	Unk_BE();
	virtual void	Unk_BF();
	virtual void	Unk_C0();
	virtual void	Unk_C1();
	virtual void	Unk_C2();
	virtual void	Unk_C3();
	virtual void	Unk_C4();
	virtual void	Unk_C5();
	virtual void	Unk_C6();
	virtual void	Unk_C7();
	virtual void	Unk_C8();
	virtual void	Unk_C9();
	virtual void	Unk_CA();
	virtual void	Unk_CB();
	virtual void	Unk_CC();
	virtual void	Unk_CD();
	virtual void	Unk_CE();
	virtual void	Unk_CF();
	virtual void	Unk_D0();
	virtual void	Unk_D1();
	virtual void	Unk_D2();
	virtual void	Unk_D3();
	virtual void	Unk_D4();
	virtual void	Unk_D5();
	virtual void	Unk_D6();
	virtual void	Unk_D7();
	virtual void	Unk_D8();
	virtual void	Unk_D9();
	virtual void	Unk_DA();
	virtual void	Unk_DB();
	virtual void	Unk_DC();
	virtual void	Unk_DD();
	virtual void	Unk_DE();
	virtual void	Unk_DF();
	virtual void	Unk_E0();
	virtual void	Unk_E1();
	virtual void	Unk_E2();
	virtual void	Unk_E3();
	virtual void	Unk_E4();
	virtual void	Unk_E5();
	virtual void	Unk_E6();
	virtual void	Unk_E7();
	virtual void	Unk_E8();
	virtual void	Unk_E9();
	virtual void	Unk_EA();
	virtual void	Unk_EB();
	virtual void	Unk_EC();
	virtual void	Unk_ED();
	virtual void	Unk_EE();
	virtual void	Unk_EF();
	virtual void	Unk_F0();
	virtual void	Unk_F1();
	virtual void	Unk_F2();
	virtual void	Unk_F3();
	virtual void	Unk_F4();
	virtual void	Unk_F5();
	virtual void	Unk_F6();
	virtual void	Unk_F7();
	virtual SInt16	GetCurrentAnimAction();
	virtual void	Unk_F9();
	virtual void	Unk_FA();
	virtual void	Unk_FB();
	virtual void	Unk_FC();
	virtual void	Unk_FD();
	virtual void	Unk_FE();
	virtual void	Unk_FF();
	virtual bool	IsAiming();
	virtual void	Unk_101();
	virtual UInt32	GetKnockedState();
	virtual void	Unk_103();
	virtual void	Unk_104();
	virtual void	Unk_105();
	virtual void	Unk_106();
	virtual void	Unk_107();
	virtual void	Unk_108();
	virtual void	Unk_109();
	virtual void	Unk_10A();
	virtual void	Unk_10B();
	virtual void	Unk_10C();
	virtual void	Unk_10D();
	virtual void	Unk_10E();
	virtual void	Unk_10F();
	virtual void	Unk_110();
	virtual void	Unk_111();
	virtual void	Unk_112();
	virtual void	Unk_113();
	virtual bool	IsWeaponOut();
	virtual void Unk_115();
	virtual void Unk_116();
	virtual void Unk_117();
	virtual void Unk_118();
	virtual void Unk_119();
	virtual void Unk_11A();
	virtual void Unk_11B();
	virtual void Unk_11C();
	virtual void Unk_11D();
	virtual void Unk_11E();
	virtual void Unk_11F();
	virtual void Unk_120();
	virtual void Unk_121();
	virtual void Unk_122();
	virtual void Unk_123();
	virtual void Unk_124();
	virtual void Unk_125();
	virtual void Unk_126();
	virtual void Unk_127();
	virtual void Unk_128();
	virtual void Unk_129();
	virtual void Unk_12A();
	virtual void Unk_12B();
	virtual void Unk_12C();
	virtual void Unk_12D();
	virtual void Unk_12E();
	virtual void Unk_12F();
	virtual void Unk_130();
	virtual void Unk_131();
	virtual void Unk_132();
	virtual void* GetFurnitureMark();
	virtual void Unk_134();
	virtual void Unk_135();
	virtual void Unk_136();
	virtual void Unk_137();
	virtual void Unk_138();
	virtual void Unk_139();
	virtual void Unk_13A();
	virtual void Unk_13B();
	virtual void Unk_13C();
	virtual void Unk_13D();
	virtual void Unk_13E();
	virtual void Unk_13F();
	virtual void Unk_140();
	virtual void Unk_141();
	virtual void Unk_142();
	virtual void Unk_143();
	virtual void Unk_144();
	virtual void Unk_145();
	virtual void Unk_146();
	virtual void Unk_147();
	virtual void Unk_148();
	virtual Actor* GetCommandingActor();
	virtual void Unk_14A();
	virtual void Unk_14B();
	virtual void Unk_14C();
	virtual void Unk_14D();
	virtual void Unk_14E();
	virtual void Unk_14F();
	virtual void Unk_150();
	virtual void Unk_151();
	virtual void Unk_152();
	virtual void Unk_153();
	virtual void Unk_154();
	virtual void Unk_155();
	virtual void Unk_156();
	virtual void Unk_157();
	virtual void Unk_158();
	virtual void Unk_159();
	virtual void Unk_15A();
	virtual void Unk_15B();
	virtual void Unk_15C();
	virtual void Unk_15D();
	virtual void Unk_15E();
	virtual void Unk_15F();
	virtual void Unk_160();
	virtual void Unk_161();
	virtual void Unk_162();
	virtual void Unk_163();
	virtual void Unk_164();
	virtual void Unk_165();
	virtual void Unk_166();
	virtual void Unk_167();
	virtual void Unk_168();
	virtual void Unk_169();
	virtual void Unk_16A();
	virtual void Unk_16B();
	virtual void Unk_16C();
	virtual void Unk_16D();
	virtual void Unk_16E();
	virtual void Unk_16F();
	virtual void Unk_170();
	virtual void Unk_171();
	virtual void Unk_172();
	virtual void Unk_173();
	virtual void Unk_174();
	virtual void Unk_175();
	virtual void Unk_176();
	virtual void Unk_177();
	virtual void Unk_178();
	virtual void Unk_179();
	virtual void Unk_17A();
	virtual void Unk_17B();
	virtual void Unk_17C();
	virtual void Unk_17D();
	virtual void Unk_17E();
	virtual void Unk_17F();
	virtual void Unk_180();
	virtual void Unk_181();
	virtual UInt32 GetFadeState();
	virtual void SetQueuedIdleFlag(UInt32 flags);
	virtual UInt32 GetQueuedIdleFlags();
	virtual void ResetQueuedIdleFlags();
	virtual void Unk_186();
	virtual void Unk_187();
	virtual void Unk_188();
	virtual void Unk_189();
	virtual void Unk_18A();
	virtual void Unk_18B();
	virtual void Unk_18C();
	virtual void Unk_18D();
	virtual void Unk_18E();
	virtual void Unk_18F();
	virtual void Unk_190();
	virtual void Unk_191();
	virtual void Unk_192();
	virtual void Unk_193();
	virtual void Unk_194();
	virtual void Unk_195();
	virtual void Unk_196();
	virtual void Unk_197();
	virtual void Unk_198();
	virtual void Unk_199();
	virtual void Unk_19A();
	virtual void Unk_19B();
	virtual void Unk_19C();
	virtual void Unk_19D();
	virtual void Unk_19E();
	virtual void Unk_19F();
	virtual void Unk_1A0();
	virtual void Unk_1A1();
	virtual void Unk_1A2();
	virtual void Unk_1A3();
	virtual void Unk_1A4();
	virtual void Unk_1A5();
	virtual void Unk_1A6();
	virtual void Unk_1A7();
	virtual void Unk_1A8();
	virtual void Unk_1A9();
	virtual void Unk_1AA();
	virtual void Unk_1AB();
	virtual void Unk_1AC();
	virtual void Unk_1AD();
	virtual void Unk_1AE();
	virtual void Unk_1AF();
	virtual void Unk_1B0();
	virtual void Unk_1B1();
	virtual void Unk_1B2();
	virtual void Unk_1B3();
	virtual void Unk_1B4();
	virtual void Unk_1B5();
	virtual void Unk_1B6();
	virtual void Unk_1B7();
	virtual void Unk_1B8();
	virtual void Unk_1B9();
	virtual void Unk_1BA();
	virtual void Unk_1BB();
	virtual void Unk_1BC();
	virtual void Unk_1BD();
	virtual void Unk_1BE();
	virtual void Unk_1BF();
	virtual void Unk_1C0();
	virtual void Unk_1C1();
	virtual void Unk_1C2();
	virtual TESIdleForm* GetForcedIdleForm();
	virtual void SetForcedIdleForm(TESIdleForm* form);
	virtual void Unk_1C5();
	virtual void Unk_1C6();
	virtual void Unk_1C7();
	virtual void Unk_1C8();
	virtual void Unk_1C9();
	virtual void Unk_1CA();
	virtual void Unk_1CB();
	virtual void Unk_1CC();
	virtual void Unk_1CD();
	virtual void Unk_1CE();
	virtual void Unk_1CF();
	virtual void Unk_1D0();
	virtual void Unk_1D1();
	virtual void Unk_1D2();
	virtual void Unk_1D3();
	virtual void Unk_1D4();
	virtual void Unk_1D5();
	virtual void Unk_1D6();
	virtual void Unk_1D7();
	virtual float GetRadsSec();
	virtual void Unk_1D9();
	virtual void Unk_1DA();
	virtual void Unk_1DB();
	virtual void Unk_1DC();
	virtual void Unk_1DD();
	virtual void Unk_1DE();
	virtual void Unk_1DF();
	virtual void Unk_1E0();
	virtual void Unk_1E1();
	virtual void Unk_1E2();
	virtual void Unk_1E3();
	virtual void Unk_1E4();
	virtual void Unk_1E5();
	virtual void Unk_1E6();
	virtual void Unk_1E7();
	
	PackageInfo		currentPackage;	// 04
	float			unk1C;			// 1C	not initialized, only by descendant!
	float			fHourLastProcessed;			// 20	not initialized, only by descendant to -1.0! flt020 gets set to GameHour minus one on package evaluation
	UInt32			iDateLastProcessed;			// 24	not initialized, only by descendant!
	UInt32			uiProcessLevel;	// 28	not initialized, only by descendant to 3 for Low, 2 for MidlleLow, 1 MiddleHighProcess and 0 for HigProcess
	CachedValues*	cachedValues;			// 2C
};

STATIC_ASSERT(sizeof(BaseProcess) == 0x30);

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
	virtual void	Unk_1E8();
	virtual void	Unk_1E9();
	virtual void	Unk_1EA();
	virtual void	Unk_1EB();
	virtual void	Unk_1EC();
	virtual void	Unk_1ED();
	virtual void	Unk_1EE();
	virtual void	Unk_1EF();
	virtual void	Unk_1F0();
	virtual void	Unk_1F1();
	virtual void	Unk_1F2();
	virtual void	Unk_1F3();
	virtual void	Unk_1F4();
	virtual void	Unk_1F5();
	virtual void	Unk_1F6();
	virtual void	Unk_1F7();
	virtual void	Unk_1F8();
	virtual void	Unk_1F9();
	virtual void	Unk_1FA();
	virtual void	Unk_1FB();
	virtual void	Unk_1FC();
	virtual void	Unk_1FD();
	virtual void	Unk_1FE();
	virtual void	Unk_1FF();
	virtual void	Unk_200();
	virtual void	Unk_201();

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
	ActorValueModifiers *damageModifiers;
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

STATIC_ASSERT(sizeof(LowProcess) == 0xB4);

class MiddleLowProcess : public LowProcess
{
public:
	MiddleLowProcess();
	~MiddleLowProcess();
	virtual void	Unk_202();

	UInt32 iHourPackageEvaluated;
	ActorValueModifiers tempModifiers;
};
STATIC_ASSERT(sizeof(MiddleLowProcess) == 0xC8);

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
	virtual void	Unk_203();
	virtual void	Unk_204();
	virtual void	Unk_205();
	virtual void	Unk_206();
	virtual void	Unk_207();
	virtual void	Unk_208();
	virtual void	Unk_209();
	virtual void	Unk_20A();
	virtual void	Unk_20B();
	virtual void	Unk_20C();
	virtual void	Unk_20D();
	virtual void	Unk_20E();
	virtual void	Unk_20F();
	virtual void	Unk_210();
	virtual void	Unk_211();
	virtual void	Unk_212();
	virtual void	Unk_213();
	virtual void	Unk_214();
	virtual void	Unk_215();
	virtual void	Unk_216();

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
	UInt8 gape111[3];
	void* pWeaponInfo;
	void* pAmmoInfo;
	void* spQueuedFile;
	void* spCloneReserver;
	UInt8 bIsUsingOneHandGrenade;
	UInt8 bIsUsingOneHandMine;
	UInt8 bIsUsingOneHandThrown;
	UInt8 isWearingHeavyArmor;
	UInt8 isWearingPowerArmorTorso;
	UInt8 isWearingPowerArmorHelmet;
	UInt8 isWearingBackpack;
	UInt8 gap12B;
	NiNode* pWeaponNode;
	void* class130;
	UInt8 wantsWeaponOut;
	bool bIsWeaponOut;
	UInt8 byt136;
	UInt8 byt137;
	bhkCharacterController* spCharCtrl;
	KnockState eKnockedState;
	UInt8 ucSitSleepState;
	UInt8 gap13E[2];
	TESObjectREFR* pFurnitureRef;
	UInt8 ucMarkerIndex;
	UInt8 gap145[3];
	FurnitureMark kFurnitureMark;
	Actor* commandingActor;
	UInt32 unk15C;
	MagicItem* magicItem160;
	UInt32 unk164;
	UInt8 byte168;
	UInt8 gap169[3];
	UInt32 pEnchantEffectShader;
	float fActorAlpha;
	float unk174;
	void* pFaceAnimationData;
	UInt8 byte17C;
	UInt8 byte17D;
	UInt8 byte17E;
	UInt8 byte17F;
	UInt8 byte180;
	UInt8 gap181[3];
	UInt32 flags184;
	UInt8 hasCaughtPlayerPickpocketting;
	UInt8 byte189;
	UInt8 bHasLifeDetectedEffect;
	UInt8 byte18B;
	UInt8 flags18C;
	UInt8 bForceNextUpdate;
	UInt8 gap18E[2];
	UInt32 iPackageIdleNumber;
	TESIdleForm* pCurrentIdle;
	float fPackageIdleTimer;
	UInt8 byte19C;
	UInt8 byte19D;
	UInt8 gap19E[2];
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
STATIC_ASSERT(sizeof(MiddleHighProcess) == 0x254);

struct DialogueItem;

class HighProcess : public MiddleHighProcess
{
public:
	HighProcess();
	~HighProcess();
	virtual void	Unk_217();

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
	HighProcess::FADE_STATE eFadeState;
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

STATIC_ASSERT(sizeof(HighProcess) == 0x45C);
STATIC_ASSERT(offsetof(HighProcess, forcedIdleForm) == 0x348);
STATIC_ASSERT(offsetof(HighProcess, queuedIdleFlags) == 0x414);
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
	virtual TESObjectCELL *	GetChildCell(void);
};
class hkpRigidBody;
class bhkPhantom;
struct AnimData;
class TESObjectREFR : public TESForm
{
public:
	MEMBER_FN_PREFIX(TESObjectREFR);
	DEFINE_MEMBER_FN(IsOffLimitsToPlayer, bool, kTESObjectREFR_IsOffLimitsToPlayerAddr);

	TESObjectREFR();
	~TESObjectREFR();
	virtual void		Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void		Unk_4F(void);
	virtual void		Unk_50(void);
	virtual void		Unk_51(void);
	virtual bool		CastShadows();
	virtual void		Unk_53(void);
	virtual void		Unk_54(void);
	virtual void		Unk_55(void);
	virtual void		Unk_56(void);
	virtual bool		IsObstacle();
	virtual void		Unk_58(void);
	virtual void		Unk_59(void);
	virtual void		Unk_5A(void);
	virtual void		Unk_5B(void);
	virtual void		Unk_5C(void);
	virtual void		Unk_5D(void);
	virtual void		Unk_5E(void);
	virtual TESObjectREFR* RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 quantity, bool keepOwner, bool drop, TESObjectREFR* destRef,
		UInt32 unk6, UInt32 unk7, bool unk8, bool unk9);
	virtual void		Unk_60(void);
	virtual bool		EquipObject(TESForm* item, UInt32 count, ExtraDataList* xData, bool lockEquip);
	virtual void		Unk_62(void);
	virtual void		Unk_63(void);
	virtual void		AddItem(TESForm* item, ExtraDataList* xDataList, UInt32 quantity);
	virtual void		Unk_65(void);
	virtual void		Unk_66(void);
	virtual void		Unk_67(void);					// Actor: GetMagicEffectList
	virtual bool		GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	virtual UInt32		GetActorUnk0148();			// result can be interchanged with baseForm, so TESForm* ?
	virtual void		SetActorUnk0148(UInt32 arg0);
	virtual void		Unk_6B(void);
	virtual void		Unk_6C(void);	// REFR: GetBSFaceGenNiNodeSkinned
	virtual void		Unk_6D(void);	// REFR: calls 006C
	virtual void		Unk_6E(void);	// MobileActor: calls 006D then NiNode::Func0040
	virtual void		Unk_6F(void);
	virtual bool		Unload3D();
	virtual void		AnimateNiNode();
	virtual void		GenerateNiNode(bool arg1);
	virtual void		Set3D(NiNode* niNode, bool unloadArt);
	virtual NiNode* GetNiNode();
	virtual void		Unk_75(void);
	virtual void		Unk_76(void);
	virtual void		Unk_77(void);
	virtual void		Unk_78(void);
	virtual AnimData* GetAnimData();			// 0079
	virtual void* GetValidBip01Names(void);	// 007A	Character only
	virtual void* CallGetValidBip01Names(void);
	virtual void		SetValidBip01Names(void* validBip01Names);
	virtual NiPoint3*		GetPos();				// GetPos or GetDistance
	virtual void		Unk_7E(UInt32 arg0);
	virtual void		Unk_7F(void);
	virtual void		Unk_80(UInt32 arg0);
	virtual void		Unk_81(UInt32 arg0);
	virtual void		Unk_82(void);
	virtual UInt32		Unk_83(void);
	virtual void		Unk_84(UInt32 arg0);
	virtual UInt32		Unk_85(void);
	virtual bool		IsCharacter();			// return false for Actor and Creature, true for character and PlayerCharacter
	virtual bool		IsCreature();
	virtual bool		IsExplosion();
	virtual bool		IsProjectile();
	virtual void		Unk_8A(void);			// SetParentCell (Interior only ?)
	virtual bool		IsDying(bool arg0);	// HasHealth (baseForm health > 0 or Flags bit23 set)
	virtual bool		Unk_8C(void);
	virtual bool		Unk_8D(void);
	virtual void		Unk_8E(void);
	virtual void		Unk_8F(void);

	enum {
		kFlags_Taken		= 0x00000022,		// 2 bits TODO: which bit does what?
		kFlags_Persistent	= 0x00000400,		//shared bit with kFormFlags_QuestItem
	};

	struct RenderState
	{
		TESObjectREFR* currWaterRef;		// 00
		UInt32			underwaterCount;	// 04	0-0x13 when fully-underwater; exterior only
		float			waterLevel;			// 08
		float			revealDistance;		// 0C
		UInt32			flags;				// 10
		NiNode* rootNode;			// 14
		bhkPhantom* phantom;			// 18	Used with trigger volume
	};

	TESChildCell	childCell;				// 018
	TESForm*		baseForm;				// 01C
	
	float			rotX, rotY, rotZ;		// 020 - either public or accessed via simple inline accessor common to all child classes
	float			posX, posY, posZ;		// 02C - seems to be private
	float			scale;					// 038 

	TESObjectCELL*	parentCell;				// 03C
	ExtraDataList	extraDataList;			// 040
	RenderState*	loadedData;				// 05C

	ScriptEventList* GetEventList() const;

	bool IsTaken() const {  return ((flags & kFlags_Taken) == kFlags_Taken) ? true : false; } // Need to implement
	bool IsPersistent() const { return ((flags & kFlags_Persistent) == kFlags_Persistent) ? true : false; }
	NiAVObject* GetNiBlock(const char* blockName) const;
	NiAVObject* GetNiBlock2(const char* blockName) const;
	NiNode* GetNode(const char* nodeName) const;
	NiNode* GetNode2(const char* nodeName) const;
	hkpRigidBody* GetRigidBody(const char* blockName) const;
	bhkCharacterController* GetCharacterController() const;
	void MoveToCell(TESObjectCELL* cell, const NiVector3& posVector);
	void SetPos(const NiVector3& posVector);


};
STATIC_ASSERT(offsetof(TESObjectREFR, baseForm) == 0x01C);
STATIC_ASSERT(sizeof(TESObjectREFR) == 0x60);
extern TESObjectREFR* s_tempPosMarker;
// 080
class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();
	virtual void		Unk_90(void);
	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Jump(void);
	virtual void		Unk_95(void);
	virtual void		Unk_96(void);
	virtual void		Unk_97(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		Unk_A0(void);
	virtual void		Unk_A1(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		Unk_AA(void);
	virtual void		Unk_AB(void);
	virtual void		Unk_AC(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual float		AdjustRot(UInt32 arg1);
	virtual void		Unk_B0(void);
	virtual void		Unk_B1(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		Unk_B4(void);
	virtual void		Unk_B5(void);
	virtual void		Unk_B6(void);
	virtual void		Unk_B7(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		IsLifeStateDying(void);
	virtual void		Unk_BB(void);
	virtual void		ToggleCollisionDebug(void);
	virtual void		Unk_BD(void);
	virtual void		Update(void);
	virtual void		Unk_BF(void);
	virtual void		GetActivatorCloseSound(void);

	BaseProcess*	 baseProcess;					// 060
	UInt32 unk064[(0x080 - 0x064) >> 2];	// 064
	// 080
};
STATIC_ASSERT(sizeof(MobileObject) == 0x080);

// 00C
class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	UInt32 vtabl;
	UInt32 unk004[2];	// 004
};

STATIC_ASSERT(sizeof(MagicCaster) == 0x00C);



class Actor : public MobileObject
{
public:
	Actor();
	~Actor();
	virtual void		Unk_C1(void);
	virtual void		Unk_C2(void);
	virtual void		Unk_C3(void);
	virtual void		Unk_C4(void);
	virtual void		Unk_C5(void);
	virtual void		Unk_C6(void);
	virtual void		SetIgnoreCrime(bool ignoreCrime);
	virtual bool		GetIgnoreCrime(void);
	virtual void		Unk_C9(void);
	virtual void		Unk_CA(void);
	virtual void		Unk_CB(void);
	virtual void		Unk_CC(void);
	virtual void		Unk_CD(void);
	virtual void		Unk_CE(void);
	virtual void		Unk_CF(void);
	virtual void		DamageActionPoints(float amount);
	virtual void		Unk_D1(void);
	virtual void		Unk_D2(void);
	virtual void		Unk_D3(void);
	virtual void		Unk_D4(void);
	virtual void		Unk_D5(void);
	virtual void		Unk_D6(void);
	virtual void		Unk_D7(void);
	virtual bool		IsPlayerRef(void);
	virtual void		Unk_D9(void);
	virtual void		Unk_DA(void);
	virtual void		Unk_DB(void);
	virtual void		Unk_DC(void);
	virtual void		Unk_DD(void);
	virtual void		Unk_DE(void);
	virtual void		Unk_DF(void);
	virtual void		Unk_E0(void);
	virtual void		Unk_E1(void);
	virtual void		Unk_E2(void);
	virtual UInt32		GetActorType(void);	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		SetActorValue(UInt32 avCode, float value);
	virtual void		SetActorValueInt(UInt32 avCode, UInt32 value);
	virtual void		Unk_E6(void);
	virtual void		Unk_E7(void);
	virtual void		Unk_E8(void);
	virtual void		Unk_E9(void);
	virtual void		ModActorValue(UInt32 avCode, int modifier, UInt32 arg3);
	virtual void		DamageActorValue(UInt32 avCode, float damage, Actor* attacker);
	virtual void		Unk_EC(void);
	virtual void		Unk_ED(void);
	virtual void		GetPreferredWeapon(void);
	virtual void		Unk_EF(void);
	virtual void		Unk_F0(void);
	virtual void		DamageItem(void);
	virtual void		Unk_F2(void);
	virtual void		HandlePickupItem(TESObjectREFR* pickedUp, int count, char a4);
	virtual void		Unk_F4(void);
	virtual void		Unk_F5(void);
	virtual void		Unk_F6(void);
	virtual void		Unk_F7(void);
	virtual void		Unk_F8(void);
	virtual void		Unk_F9(void);
	virtual void		Unk_FA(void);
	virtual void		Unk_FB(void);
	virtual void		Unk_FC(void);
	virtual void		Unk_FD(void);
	virtual void		Unk_FE(void);
	virtual void		Unk_FF(void);
	virtual void		Unk_100(void);
	virtual void		Unk_101(void);
	virtual void		Unk_102(void);
	virtual void		Unk_103(void);
	virtual void		Unk_104(void);
	virtual void		Unk_105(void);
	virtual void		Unk_106(void);
	virtual void		Unk_107(void);
	virtual void		Unk_108(void);
	virtual void		Unk_109(void);
	virtual void		Unk_10A(void);
	virtual void		Unk_10B(void);
	virtual void		Unk_10C(void);
	virtual void		Unk_10D(void);
	virtual void		Unk_10E(void);
	virtual void		Unk_10F(void);
	virtual void		Unk_110(void);
	virtual void		Unk_111(void);
	virtual void		Unk_112(void);
	virtual void		Unk_113(void);
	virtual void		Unk_114(void);
	virtual void		Unk_115(void);
	virtual void		Unk_116(void);
	virtual void		Unk_117(void);
	virtual void		Unk_118(void);
	virtual void		Unk_119(void);
	virtual void		Unk_11A(void);
	virtual void		Unk_11B(void);
	virtual void		Unk_11C(void);
	virtual void		Unk_11D(void);
	virtual void		Unk_11E(void);
	virtual void		Unk_11F(void);
	virtual void		Unk_120(void);
	virtual void		Unk_121(void);
	virtual void		Unk_122(void);
	virtual void		Unk_123(void);
	virtual void		Unk_124(void);
	virtual void		Unk_125(void);
	virtual void		Unk_126(void);
	virtual void		Unk_127(void);
	virtual void		Unk_128(void);
	virtual void		Unk_129(void);
	virtual void		Unk_12A(void);
	virtual void		Unk_12B(void);
	virtual void		Unk_12C(void);
	virtual void		Unk_12D(void);
	virtual void		Unk_12E(void);
	virtual void		Unk_12F(void);
	virtual void		Unk_130(void);
	virtual void		Unk_131(void);
	virtual void		GetAnticipatedLocation(void);

	MagicCaster			magicCaster;			// 080
	MagicTarget			magicTarget;			// 08C
	ActorValueOwner		avOwner;				// 09C
	CachedValuesOwner	cvOwner;				// 0A0

	bhkRagdollController* ragDollController;			// 0A4
	bhkRagdollPenetrationUtil* ragDollPentrationUtil;	// 0A8
	UInt32				unk0AC[(0xB8 - 0x0AC) >> 2];	// 0AC
	Actor*				killer;							// 0B8
	UInt32				unk0BC[(0xF8 - 0x0BC) >> 2];	// 0BC
	bool				inCombat;						// 0F8
	UInt32				lifeState;						// 0FC
	UInt32				unk100[(0x138 - 0x0100) >> 2];	// 100
	bool				ignoreCrime;					// 138
	UInt8				byte139;						// 139
	UInt8				byte13A;						// 13A
	UInt8				byte13B;						// 13B
	UInt32				unk13C;							// 13C
	UInt8				inWater;						// 140
	UInt8				isSwimming;						// 141
	UInt16				unk142;							// 142
	UInt32				unk144[(0x184 - 0x144) >> 2];	// 144
	void*			actorMover;						// 184
	UInt32				unk188[(0x19C - 0x188) >> 2];	// 188
	UInt32				sitSleepState;

	bool IsEssential() { return ThisCall<bool>(0x6F4A10, this); }
	float GetRadiationLevel(bool scaleByResist);
	TESActorBase* GetActorBase();
	SInt8 GetKnockedState() { return (baseProcess && (baseProcess->uiProcessLevel <= 1)) ? ((MiddleHighProcess*)baseProcess)->eKnockedState : -1; }
};

STATIC_ASSERT(offsetof(Actor, cvOwner) == 0x0A0);
STATIC_ASSERT(sizeof(Actor) == 0x1A0);
class BipedAnim {
public:
	enum eOptionalBoneType {
		kOptionalBone_Bip01Head = 0,
		kOptionalBone_Weapon = 1,
		kOptionalBone_Bip01LForeTwist = 2,
		kOptionalBone_Bip01Spine2 = 3,
		kOptionalBone_Bip01Neck1 = 4,
	};

	// 008
	struct OptionalBone
	{
		bool	bExists;
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
STATIC_ASSERT(sizeof(BipedAnim) == 0x2B4);
class Character : public Actor
{
public:
	Character();
	~Character();

	BipedAnim* bipedAnim;
	float fTotalArmorDT;
	UInt8 bIsTrespassing;
	UInt8 bIsGuard;
	UInt16 unk1C2;
	float fFlyInventoryWeight;
};
STATIC_ASSERT(sizeof(Character) == 0x1B0);
// 9BC

struct CompassTarget
{
	Actor* target;
	UInt8 isHostile;
	UInt8 isUndetected;
};
STATIC_ASSERT(sizeof(CompassTarget) == 8);



class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};
	struct WobbleNodes
	{
		NiNode* wobbleAnimNodes[12];
		NiNode* wobbleAnimNodes2[12];
	};

	// lotsa data
	UInt32								unk188[(0x570-0x1B0) >> 2];		// 188
	BGSNote								* note;							// 570
	UInt32								unk574;							// 574
	ImageSpaceModifierInstanceDOF		* unk578;						// 578
	ImageSpaceModifierInstanceDOF		* unk57C;						// 57C
	ImageSpaceModifierInstanceDRB		* unk580;						// 580
	UInt32								unk584[(0x5A8 - 0x584) >> 2];	// 584
	bool								bIs3rdPerson;
	bool								bThirdPerson;					// 5A9
	UInt8								unk5A9[2];	
	UInt32								unk5AC[(0x5DC - 0x5AC) >> 2];	// 5AC
	UInt32								disabledControlFlags;			// 5DC
	UInt32								unk5E0;							// 5E0
	UInt32								unk5E4;							// 5E4
	BipedAnim*							pBipedAnims1st;					// 5E8
	UInt32								unk5EC;							// 5EC
	NiNode								* unk5F0;						// 5F0
	float								eyeHeight;						// 5F4
	UInt32								unk5F8[(0x604-0x5F8) >> 2];		// 5F8
	TESTopic							* topic;						// 604
	UInt32								unk608[3];						// 608
	TESQuest							* quest;						// 614
	BGSQuestObjective					* questObjective;				// 618
	UInt32								unk61C[(0x68C - 0x61C) >> 2];	// 61C
	float								timeGrenadeHeld;				// 68C
	UInt8								unk690[(0x704 - 0x690)];		// 690
	UInt32								gameDifficulty;					// 704
	UInt8								unk708[(0x718 - 0x708)];		// 708
	float								flycamZRot;						// 718
	float								flycamXRot;						// 71C
	float								flycamPosX;						// 720
	float								flycamPosY;						// 724
	float								flycamPosZ;						// 728
	UInt8								unk72C[440];
	Actor*								autoAimActor;
	float								kBulletAutoAim[3];
	UInt32								unk8F4;
	bool								bIsActorWithinPickDistance;
	Actor*								reticleActor;
	tList<CompassTarget>				*compassTargets;
	float								fPipboyLightHeldTime;
	bool								bShouldOpenPipboy;
	char								byteD55;
	char								byteD56;
	char								byteD57;
	NiPoint3							camera3rdPos;
	void*								pCombatGroup;
	int									unk91C;
	int									iTeammateCount;
	float								fCombatYieldRetryTimer;
	PlayerCharacter::WobbleNodes		wobbleAnims;
	NiVector3							kCamera1stPos;
	NiVector3							kCameraPos;
	void*								spRigidBody;
	bool								pcInCombat;						// 9A4
	bool								pcUnseen;						// 9A5
	UInt8								unk9A6[(0x9BC - 0x9A6)];		// 9A6

	static PlayerCharacter*	GetSingleton();
}; 

STATIC_ASSERT(sizeof(PlayerCharacter) == 0x9BC);
STATIC_ASSERT(offsetof(PlayerCharacter, fCombatYieldRetryTimer) == 0x924);
STATIC_ASSERT(offsetof(PlayerCharacter, bThirdPerson) == 0x5A9);
STATIC_ASSERT(offsetof(PlayerCharacter, disabledControlFlags) == 0x5DC);
STATIC_ASSERT(offsetof(PlayerCharacter, camera3rdPos) == 0x90C);

struct VATSCameraData
{
	tList<void>						targetsList;	// 00
	UInt32							mode;			// 08
	UInt32							unk0C;			// 0C
	BGSCameraShot* camShot;		// 10
	float							flt14;			// 14
	float							flt18;			// 18
	UInt32							unk1C;			// 1C
	UInt32							unk20;			// 20
	UInt32							unk24;			// 24
	void*							isModInstForm;	// 28
	void*							isModInstRB;		// 2C
	UInt32							unk30;			// 30
	void*							object34;		// 34
	UInt8							byte38;			// 38
	UInt8							pad39[3];		// 39
	UInt32							unk3C;			// 3C
	UInt32							unk40;			// 40
	UInt32							unk44;			// 44
	void Free() { ThisCall(0x7CF2A0, this); }
	static VATSCameraData* GetSingleton() { return (VATSCameraData*)0x108D098; };
};
STATIC_ASSERT(sizeof(VATSCameraData) == 0x48);

// 18
class LoadedReferenceMap : public NiTPointerMap<TESObjectREFR>
{
public:
	UInt32			unk10;		// 10
	UInt32			unk14;		// 14
};

// C0
struct LoadedReferenceCollection
{
	LoadedReferenceMap	map00;					// 00	Unused?
	LoadedReferenceMap	allInventoryObjects;	// 18
	LoadedReferenceMap	weaponObjects;			// 30
	LoadedReferenceMap	ammoObjects;			// 48
	LoadedReferenceMap	containers;				// 60
	LoadedReferenceMap	actors;					// 78
	LoadedReferenceMap	radiationEmitters;		// 90
	LoadedReferenceMap	teleportDoors;			// A8

	__forceinline static LoadedReferenceCollection* Get() { return *(LoadedReferenceCollection**)0x1070B34; }
};

class BSGameSound
{
public:
	/*00*/virtual BSGameSound* Destroy(bool doFree);
	/*04*/virtual bool	CheckStateFlagsBit5();
	/*08*/virtual bool	CheckStateFlagsBit19();
	/*0C*/virtual bool	CheckStateFlagsBit6();
	/*10*/virtual float	GetVolume();
	/*14*/virtual void	SetIsLooping(bool doSet);
	/*18*/virtual void	Unk_06(void);
	/*1C*/virtual void	Unk_07(void);
	/*20*/virtual void	Unk_08(void);
	/*24*/virtual void	Unk_09(void);
	/*28*/virtual void	Unk_0A(void);
	/*2C*/virtual void	CopyFrom(BSGameSound* source, bool arg2);
	/*30*/virtual void	Unk_0C(void);
	/*34*/virtual bool	Unk_0D(void);
	/*38*/virtual bool	Unk_0E(void);
	/*3C*/virtual bool	SetVolume(float inVol);
	/*40*/virtual void	Unk_10(void);
	/*44*/virtual bool	Unk_11(void);
	/*48*/virtual void	Unk_12(void);
	/*4C*/virtual void	Unk_13(float arg1, float arg2, float arg3);
	/*50*/virtual void	Unk_14(float arg1, float arg2, float arg3);
	/*54*/virtual void	Unk_15(void* arg1);
	/*58*/virtual void	Unk_16(void);
	/*5C*/virtual void	Unk_17(float arg1, float arg2);
	/*60*/virtual void	Unk_18(UInt16 arg1, UInt16 arg2, UInt16 arg3, UInt16 arg4, UInt16 arg5);
	/*64*/virtual bool	SetFrameFrequencyPerc(float freqPerc);
	/*68*/virtual float	GetFrameFrequencyPerc();
	/*6C*/virtual void	Seek(UInt32 timePoint);

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

	UInt32			mapKey;					// 004
	UInt32			soundFlags;				// 008
	UInt32			flags00C;				// 00C
	UInt32			stateFlags;				// 010
	UInt32			duration;				// 014
	UInt16			staticAttenuation;		// 018	dB * -1000
	UInt16			unk01A;					// 01A
	UInt16			unk01C;					// 01C
	UInt16			unk01E;					// 01E
	UInt16			unk020;					// 020
	UInt16			unk022;					// 022
	float			volume;					// 024	0.0 to 1.0
	float			flt028;					// 028
	float			flt02C;					// 02C
	UInt32			unk030;					// 030
	UInt16			baseSamplingFreq;		// 034
	char			filePath[254];			// 036	Originally: filePath[260]
	TESSound* sourceSound;			// 134	"Stolen" from filePath
	float			frequencyMod;			// 138	^
	float			maxAttenuationDist;		// 13C
	float			minAttenuationDist;		// 140
	UInt32			pathHashFile;			// 144
	UInt32			pathHashFolder;			// 148
	UInt32			unk14C;					// 14C
	float			flt150;					// 150
	UInt32			unk154;					// 154
	SInt8			randFrequencyShift;		// 158
	UInt8			byte159;				// 159
	UInt16			samplingFreq;			// 15A
	UInt32			unk15C;					// 15C
	UInt32			unk160;					// 160
	UInt32			unk164;					// 164
	UInt32			unk168;					// 168
	UInt32			unk16C;					// 16C
	UInt32			unk170;					// 170
	UInt32			unk174[5];				// 174
	UInt32			priority;				// 188
	UInt32			unk18C[3];				// 18C
};
STATIC_ASSERT(sizeof(BSGameSound) == 0x198);

class BSAudioManager
{
public:
	virtual void				Destroy(bool doFree);

	// 10
	struct AudioRequest
	{
		UInt32				count;			// 00
		UInt8				byte04;			// 04
		UInt8				pad05[3];		// 05
		void*				requestData;	// 08
		UInt32				tickCount;		// 0C
	};

	UInt32						unk004[8];			// 004
	AudioRequest				request024;			// 024
	AudioRequest				request034;			// 034
	AudioRequest				request044;			// 044
	NiTPointerMap<BSGameSound>		playingSounds;		// 054
	NiTPointerMap<void>		playingSoundInfos1;	// 064
	NiTPointerMap<void>		playingSoundInfos2;	// 074
	NiTPointerMap<NiAVObject>		soundPlayingObjects;// 084
	DList<BSGameSound>			cachedSounds;		// 094
	UInt32						unk0A0;				// 0A0
	UInt32						unk0A4;				// 0A4
	float						flt0A8;				// 0A8
	CRITICAL_SECTION			cs0AC;
	CRITICAL_SECTION			cs0C4;
	CRITICAL_SECTION			cs0DC;
	CRITICAL_SECTION			cs0F4;
	UInt32						lastTickCount;
	BYTE						initedSounds;
	BYTE						unk111;
	UInt32						threadID;
	void*						updateThread;
	float						volumes[6];
	UInt32						unk134;
	UInt8						byte138;
	UInt8						byte139;
	UInt32						unk13C;
	UInt32						unk140;
	UInt32						soundCacheIndex;

	__forceinline static BSAudioManager* Get() { return (BSAudioManager*)0x11791D8; }


};
static_assert(sizeof(BSAudioManager) == 0x148);


// 04
class AnimSequenceBase
{
public:
	virtual void	Destroy(bool deFree);
	virtual void	AddAnimGroupSequence(BSAnimGroupSequence* sequence, UInt32 arg2);
	virtual bool	RemoveAnimGroupSequence(BSAnimGroupSequence* sequence, UInt32 arg2);
	virtual bool	IsSingle();
	virtual BSAnimGroupSequence* GetSequenceByIndex(UInt8 index);
	virtual BSAnimGroupSequence* GetSequenceByGroup(TESAnimGroup* animGroup);
	virtual char	GetSequenceIndexByName(const char* seqName);
};

struct KFModel;
// 38
class AnimIdle : public NiRefObject
{
public:
	UInt32					unk08;			// 08
	UInt32					unk0C;			// 0C
	UInt32					unk10;			// 10
	UInt32					sequenceID;		// 14
	BSAnimGroupSequence*	agSequence;	// 18
	NiObject*				object1C;		// 1C
	NiObject*				object20;		// 20
	NiObject*				object24;		// 24
	NiObject*				object28;		// 28
	TESIdleForm*			idleForm;		// 2C
	UInt32					unk30;			// 30
	Actor*					actor;			// 34
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
		float			replayDelay;
	};

	UInt32							unk000;				// 000
	Actor*							actor;				// 004
	NiNode*							nSceneRoot;		// 008
	NiNode*							nBip01;			// 00C
	UInt32							unk010;				// 010
	float							flt014;				// 014
	float							flt018;				// 018
	NiVector3						pos01C;				// 01C
	NiNode*							nPelvis;			// 028
	NiNode*							nBip01Copy;		// 02C
	NiNode*							nLForearm;			// 030
	NiNode*							nHead;				// 034
	NiNode*							nWeapon;			// 038
	UInt32							unk03C[2];			// 03C
	NiNode*							nNeck1;			// 044
	float							flt048;				// 048
	UInt16							animGroupIDs[8];	// 04C
	SInt32							sequenceState1[8];	// 05C
	SInt32							sequenceState2[8];	// 07C
	UInt16							word09C[8];			// 09C
	UInt32							unk0AC[8];			// 0AC
	UInt8							byte0CC;			// 0CC
	UInt8							byte0CD;			// 0CD
	UInt8							byte0CE;			// 0CE
	UInt8							byte0CF;			// 0CF
	float							timePassed;			// 0D0
	UInt32							unk0D4;				// 0D4
	NiControllerManager*			controllerMngr;	// 0D8
	NiTPointerMap<AnimSequenceBase>* sequenceBaseMap;	// 0DC
	BSAnimGroupSequence*			animSequence[8];	// 0E0
	BSAnimGroupSequence*			animSeq100;		// 100
	tList<KFModel>					loadingAnims;		// 104
	float							movementSpeedMult;	// 10C
	float							rateOfFire;			// 110
	UInt8							noBlend;			// 120
	UInt8							byte121;			// 121
	UInt16							word122;			// 122
	AnimIdle*						idleAnim;			// 124
	AnimIdle*						queuedIdleAnim;	// 128
	NiObject*						object12C;			// 12C
	NiObject*						object130;			// 130
	tList<PlayingIdle>				playingIdleAnims;	// 134

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
		if (animSequence[sequenceIdx])
			ThisCall(0x45E4C0, this, sequenceIdx, 0);
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

	static BSTreeManager* GetSingleton(bool bEnableCreation) { return CdeclCall<BSTreeManager*>(0x59BAA0, bEnableCreation); }
};

static_assert(sizeof(BSTreeManager) == 0x20);

class Sky
{
public:
	virtual Sky* Destructor(bool doFree);

	NiNode* niNode004;					// 004
	NiNode* niNode008;					// 008
	TESClimate* currClimate;			// 00C
	TESWeather* currWeather;			// 010
	TESWeather* transWeather;			// 014	Previous weather, gradually fading, on weather transition
	TESWeather* defaultWeather;			// 018	Picked from currClimate weathers list. currClimate is set to this unless there's a regional weather
	TESWeather* overrideWeather;		// 01C
	BYTE		unk020[(0x12C - 0x20)];	// 020

	__forceinline static Sky* Get() { return *(Sky**)0x107A0E8; }

	__forceinline void RefreshClimate(TESClimate* climate, bool immediate = true)
	{
		ThisCall(0x57E3B0, this, climate, immediate);
	}
};
static_assert(sizeof(Sky) == 0x12C);