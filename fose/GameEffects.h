#pragma once

class MagicTarget;
class MagicCaster;
class MagicItem;
class EffectItem;
class ActiveEffect
{
public:
	/*000*/virtual void		Destroy(bool doFree);
	/*004*/virtual ActiveEffect* Clone();
	/*008*/virtual void		Unk_02();
	/*00C*/virtual void		Unk_03(float arg);
	/*010*/virtual void		SaveGame(UInt32 arg);
	/*014*/virtual void		LoadGame(UInt32 arg);
	/*018*/virtual void		Unk_06(UInt32 arg);
	/*01C*/virtual void		Unk_07(UInt32 arg);
	/*020*/virtual void		Unk_08(UInt32 arg);
	/*024*/virtual void		Unk_09(UInt32 arg);
	/*028*/virtual void		Unk_0A(UInt32 arg);
	/*02C*/virtual void		CopyTo(ActiveEffect* _target);
	/*030*/virtual bool		Unk_0C(UInt32 arg);
	/*034*/virtual bool		Unk_0D(UInt32 arg);
	/*038*/virtual bool		ResetCasterIfSame(MagicCaster* _caster);
	/*03C*/virtual bool		Unk_0F();
	/*040*/virtual void		Unk_10(UInt32 arg);
	/*044*/virtual void		CopyFrom(ActiveEffect* from);
	/*048*/virtual bool		Unk_12(UInt32 arg);
	/*04C*/virtual bool		Unk_13(MagicTarget* _target);
	/*050*/virtual void		Unk_14();
	/*054*/virtual void		Unk_15();
	/*058*/virtual void		Unk_16();

	float			timeElapsed;		// 04
	MagicItem*		magicItem;			// 08
	EffectItem*		effectItem;		// 0C
	bool			bActive;			// 10
	bool			bApplied;			// 11
	bool			byte12;				// 12
	bool			bTerminated;		// 13 set to 1 when effect is to be removed
	UInt8			byte14;				// 14
	UInt8			pad15[3];			// 15
	UInt32			flags;				// 18
	float			magnitude;			// 1C - adjusted based on target?
	float			duration;			// 20 - adjusted based on target?
	MagicTarget*	target;			// 24
	MagicCaster*	caster;			// 28
	UInt32			spellType;			// 2C e.g. SpellItem::kType_Ability
	void*			sound[3];				// 30
	TESForm*		enchantObject;		// 3C	Object responsible for effect
	tList<void>* list40;			// 40
	UInt32			unk44;				// 44


};

STATIC_ASSERT(sizeof(ActiveEffect) == 0x48);

class ValueModifierEffect : public ActiveEffect
{
public:
	ValueModifierEffect();
	~ValueModifierEffect();
};

class AssociatedItemEffect : public ActiveEffect
{
public:
	AssociatedItemEffect();
	~AssociatedItemEffect();
};

class CommandEffect : public ActiveEffect
{
public:
	CommandEffect();
	~CommandEffect();
};

class AbsorbEffect : public ValueModifierEffect
{
public:
	AbsorbEffect();
	~AbsorbEffect();
};

class BoundItemEffect : public AssociatedItemEffect
{
public:
	BoundItemEffect();
	~BoundItemEffect();
};

class CalmEffect : public ValueModifierEffect
{
public:
	CalmEffect();
	~CalmEffect();
};

class ChameleonEffect : public ValueModifierEffect
{
public:
	ChameleonEffect();
	~ChameleonEffect();
};

class CommandCreatureEffect : public CommandEffect
{
public:
	CommandCreatureEffect();
	~CommandCreatureEffect();
};

class CommandHumanoidEffect : public CommandEffect
{
public:
	CommandHumanoidEffect();
	~CommandHumanoidEffect();
};

class ConcussionEffect : public ActiveEffect
{
public:
	ConcussionEffect();
	~ConcussionEffect();
};

class CureEffect : public ActiveEffect
{
public:
	CureEffect();
	~CureEffect();
};

class DarknessEffect : public ValueModifierEffect
{
public:
	DarknessEffect();
	~DarknessEffect();
};

class DemoralizeEffect : public ActiveEffect
{
public:
	DemoralizeEffect();
	~DemoralizeEffect();
};

class DetectLifeEffect : public ValueModifierEffect
{
public:
	DetectLifeEffect();
	~DetectLifeEffect();
};

class DisintegrateArmorEffect : public ActiveEffect
{
public:
	DisintegrateArmorEffect();
	~DisintegrateArmorEffect();
};

class DisintegrateWeaponEffect : public ActiveEffect
{
public:
	DisintegrateWeaponEffect();
	~DisintegrateWeaponEffect();
};

class DispelEffect : public ActiveEffect
{
public:
	DispelEffect();
	~DispelEffect();
};

class FrenzyEffect : public ValueModifierEffect
{
public:
	FrenzyEffect();
	~FrenzyEffect();
};

class InvisibilityEffect : public ValueModifierEffect
{
public:
	InvisibilityEffect();
	~InvisibilityEffect();
};

class LightEffect : public ActiveEffect
{
public:
	LightEffect();
	~LightEffect();
};

class LockEffect : public ActiveEffect
{
public:
	LockEffect();
	~LockEffect();
};

class NightEyeEffect : public ValueModifierEffect
{
public:
	NightEyeEffect();
	~NightEyeEffect();
};

class OpenEffect : public ActiveEffect
{
public:
	OpenEffect();
	~OpenEffect();
};

class ParalysisEffect : public ValueModifierEffect
{
public:
	ParalysisEffect();
	~ParalysisEffect();
};

class ReanimateEffect : public ActiveEffect
{
public:
	ReanimateEffect();
	~ReanimateEffect();
};

class ScriptEffect : public ActiveEffect
{
public:
	ScriptEffect();
	~ScriptEffect();
};

class ShieldEffect : public ValueModifierEffect
{
public:
	ShieldEffect();
	~ShieldEffect();
};

class SummonCreatureEffect : public AssociatedItemEffect
{
public:
	SummonCreatureEffect();
	~SummonCreatureEffect();
};

class SunDamageEffect : public ActiveEffect
{
public:
	SunDamageEffect();
	~SunDamageEffect();
};

class TelekinesisEffect : public ValueModifierEffect
{
public:
	TelekinesisEffect();
	~TelekinesisEffect();
};

class TurnUndeadEffect : public ActiveEffect
{
public:
	TurnUndeadEffect();
	~TurnUndeadEffect();
};

class ValueAndConditionsEffect : public ValueModifierEffect
{
public:
	ValueAndConditionsEffect();
	~ValueAndConditionsEffect();
};

