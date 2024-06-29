#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"

ScriptEventList* TESObjectREFR::GetEventList() const
{
	BSExtraData* xData = extraDataList.GetByType(kExtraData_Script);
	if (xData)
	{
		ExtraScript* xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript);
		if (xScript)
			return xScript->eventList;
	}

	return 0;
}


static PlayerCharacter ** g_thePlayer = (PlayerCharacter **)0x0107A104;


PlayerCharacter* PlayerCharacter::GetSingleton()
{
	return *g_thePlayer;
}


float GetDistance(TESObjectREFR* ref1, TESObjectREFR* ref2) {
	return (float)ThisCall<double>(0x4EDBF0, ref1, ref2, 0, 0);
}
float GetThresholdedAV(Actor* actor, int avCode) {
	return ThisCall<float>(0x5A28E0, &actor->avOwner, avCode);
}

float Actor::GetRadiationLevel()
{
	float result = 0;
	if (inWater)
	{
		TESWaterForm* waterForm = nullptr;
		if (loadedData && loadedData->currWaterRef)
		{
			waterForm = ((BGSPlaceableWater*)loadedData->currWaterRef->baseForm)->water;
			if (waterForm && waterForm->waterForm)
				waterForm = waterForm->waterForm;
		}
		else {
			waterForm = ThisCall<TESWaterForm*>(0x4D5710, parentCell);
		}
		if (waterForm && waterForm->radiation) {
			result = waterForm->radiation * (isSwimming ? *(float*)(0xF620CC+4) : *(float*)(0xF620D8+4));
		}
	}
	for (auto iter = LoadedReferenceCollection::Get()->radiationEmitters.Begin(); !iter.Done(); iter.Next()) {
		if (TESObjectREFR* refr = iter.Get()) {
			if (auto xRadius = (ExtraRadius*)(refr->extraDataList.GetByType(kExtraData_Radius))) {
				float distance = xRadius->radius - GetDistance((TESObjectREFR*)this, refr);
				if (distance > 0) {
					if (auto xRadiation = (ExtraRadiation*)(refr->extraDataList.GetByType(kExtraData_Radiation))) {
						result += xRadiation->radiation * distance / xRadius->radius;
					}
				}
			}
		}
	}
	if (result)
		result *= 1.0F - (GetThresholdedAV(this, eActorVal_RadResistance) * 0.01F);
	if (result < 0.0) result = 0.0;
	return result;
}
TESActorBase* Actor::GetActorBase()

{
	ExtraLeveledCreature* xLvlCre = (ExtraLeveledCreature*)this->extraDataList.GetByType(kExtraData_LeveledCreature);
	return (xLvlCre && xLvlCre->form) ? (TESActorBase*)xLvlCre->form : (TESActorBase*)baseForm;
}

