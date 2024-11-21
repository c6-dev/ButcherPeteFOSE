#include <string>
#include "GameTiles.h"
#include "Utilities.h"
#include "GameAPI.h"
#include "GameInterface.h"


DListNode<Tile>* Tile::GetNthChild(UInt32 index)
{
	return children.Tail()->Regress(index);
}

void Tile::DestroyAllChildren()
{
	ThisCall(0xBEB880, this);
}

Tile* Tile::GetChildByName(const char* _name)
{
	return ThisCall<Tile*>(0xBEABD0, this, _name);
}

void Tile::SetFloat(UInt32 id, float fltVal, bool bRemoveFromReactionMap)
{
	ThisCall(kTile_SetFloatAddr, this, id, fltVal, bRemoveFromReactionMap);
}

void Tile::SetString(const char* strVal, bool bRemoveFromReactionMap)
{
	SetString(kTileValue_string, strVal);
}

void Tile::SetString(UInt32 id, const char* strVal, bool bRemoveFromReactionMap)
{
	ThisCall(kTile_SetStringAddr, this, id, strVal, bRemoveFromReactionMap);
}

void Tile::SetVisible(bool isVisible, bool bRemoveFromReactionMap)
{
	SetFloat(kTileValue_visible, isVisible, bRemoveFromReactionMap);
}

Tile* Tile::ReadXML(const char* xmlPath)
{
	return ThisCall<Tile*>(0xBF37B0, this, xmlPath);
}

float Tile::GetValueFloat(UInt32 id)
{
	Tile::Value* val = ThisCall<Tile::Value*>(0xBECE90, this, id);
	if (val) {
		return val->num;
	}
	return 0.0;
}

void Tile::StartGradualSetTrait(TileValues trait, float startValue, float endValue, float duration, int type)
{
	ThisCall(0xBF05A0, this, trait, startValue, endValue, duration, type);
}

bool Tile::IsInGradualSetTrait(TileValues trait)
{
	return !ThisCall<bool>(0xBEACA0, this, trait);
}

void Tile::EndGradualSetTrait(UInt32 trait)
{
	ThisCall(0xBED770, this, trait);
}

float Tile::GetLocusAdjustedPosX()
{
	return ThisCall<float>(0xBECFB0, this);
}

float Tile::GetLocusAdjustedPosY()
{
	return ThisCall<float>(0xBED040, this);
}

void Tile::SetParent(Tile* newParent, Tile* oldParent)
{
	ThisCall(0xBEEA70, this, newParent, oldParent);
}

Tile* Tile::GetComponent(const char* componentPath, const char** trait)
{
	Tile* parentTile = this;
	while (char* slashPos = (char*)strchr(componentPath, '\\'))
	{
		*slashPos = 0;
		parentTile = parentTile->GetChild(componentPath);
		if (!parentTile) return nullptr;
		componentPath = slashPos + 1;
	}
	if (*componentPath)
	{
		Tile* result = parentTile->GetChild(componentPath);
		if (result) return result;
		*trait = componentPath;
	}
	return parentTile;
}

Tile::Value* Tile::GetValueName(const char* valueName) const
{
	return GetValue(TraitNameToID(valueName));
}

Tile::Value* Tile::GetValue(UInt32 typeID) const
{

	if (this->values.size > 0) {
		for (int i = 0; i < this->values.size; i++) {
			Tile::Value* val = this->values.data[i];
			if (val->id == typeID) return val;
		}
	}
	return nullptr;
}

Tile* Tile::GetTargetComponent(const char* componentPath, Tile::Value** value)
{
	char* slashPos = (char*)strchr(componentPath, '\\');
	if (!slashPos) return TileMenu::GetMenuTile(componentPath);
	*slashPos = 0;
	Tile* component = TileMenu::GetMenuTile(componentPath);
	if (!component) return nullptr;
	const char* trait = nullptr;
	component = component->GetComponent(slashPos + 1, &trait);
	if (!component) return nullptr;
	if (trait)
	{
		if (!value || !(*value = component->GetValueName(trait))) return nullptr;
	}
	else if (value) return nullptr;
	return component;
}

Tile* Tile::GetChild(const char* childName)
{
	int childIndex = 0;
	char* colon = (char*)strchr(childName, ':');
	if (colon)
	{
		if (colon == childName) return nullptr;
		*colon = 0;
		childIndex = atoi(colon + 1);
	}
	Tile* result = nullptr;
	bool wildcard = *childName == '*';
	for (auto node = children.begin(); node != children.end(); ++node)
	{
		if (*node && (wildcard || !stricmp((*node)->name.m_data, childName)) && !childIndex--)
		{
			result = *node;
			break;
		}
	}
	if (colon) *colon = ':';
	return result;
}

void Tile::Value::SetFloat(float fltVal, bool bRemoveFromReactionMap)
{
	if (this->num != fltVal) {
		this->num = fltVal;
		this->Refresh(true);
	}
}

TileMenu* TileMenu::GetMenuTile(const char* componentPath)
{
	char traitName[MAX_PATH];
	sprintf(traitName, "&%s;", componentPath);
	if (UInt32 menuID = Tile::TraitNameToID(traitName)) {
		return CdeclCall<TileMenu*>(0xBEAF00, menuID);
	}
	return nullptr;
}