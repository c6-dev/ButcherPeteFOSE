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

