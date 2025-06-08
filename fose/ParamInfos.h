#pragma once

#include "CommandTable.h"

static ParamInfo kParams_OneInt[1] =
{
	{	"int", kParamType_Integer, 0 }, 
};

static ParamInfo kParams_FourInts[4] =
{
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },

};

static ParamInfo kParams_TwoInts[2] =
{
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 0 },
};

static ParamInfo kParams_OneOptionalInt[1] =
{
	{	"int", kParamType_Integer, 1 }, 
};

static ParamInfo kParams_OneString_OneOptionalInt[2] =
{
	{ "string",	kParamType_String,	0 },
	{ "Integer", kParamType_Integer, 1 },
};

static ParamInfo kParams_OneString_OneInt[2] =
{
	{ "string",	kParamType_String,	0 },
	{ "Integer", kParamType_Integer, 0 },
};

static ParamInfo kParams_OneInt_OneOptionalInt[2] =
{
	{	"int", kParamType_Integer, 0 },
	{	"int", kParamType_Integer, 1 },
};

static ParamInfo kParams_OneFloat[1] =
{
	{	"float", kParamType_Float,	0 },
};

static ParamInfo kParams_OneOptionalActorBase[1] =
{
	{	"actorBase", kParamType_ActorBase,	1 },
};

static ParamInfo kParams_OneString_ThreeOptionalFloats_OneOptionalInt[5] =
{
	{"string", kParamType_String, 0},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"float", kParamType_Float, 1},
	{"integer", kParamType_Integer, 1}
};

static ParamInfo kParams_OneOptionalIdleForm[1] =
{
	{	"idle form", kParamType_IdleForm,	1 },
};

static ParamInfo kParams_OneIdleForm[1] =
{
	{	"idle form", kParamType_IdleForm,	0 },
};
static ParamInfo kParams_OneString[1] =
{
	{	"string",	kParamType_String,	0 },
};

static ParamInfo kParams_OneString_OneFloat[] =
{
	{	"string",	kParamType_String,	0 },
	{	"float",	kParamType_Float,	0 },
};

static ParamInfo kParams_TwoFloats[2] =
{
	{	"float",	kParamType_Float,	0 },
	{	"float",	kParamType_Float,	0 },
};

static ParamInfo kParams_OneSpellItem[1] =
{
	{	"spell", kParamType_SpellItem, 0 }, 
};

static ParamInfo kParams_OneMagicItem[1] =
{
	{	"spell", kParamType_MagicItem, 0 }, 
};

static ParamInfo kParams_OneMagicEffect[1] =
{
	{	"magic effect", kParamType_MagicEffect, 0 }, 
};

static ParamInfo kParams_OneInventoryObject[1] =
{
	{	"item", kParamType_InventoryObject, 0},
};

static ParamInfo kParams_OneOptionalInventoryObject[1] =
{
	{	"item", kParamType_InventoryObject, 1},
};

static ParamInfo kParams_OneInt_OneOptionalInventoryObject[2] =
{
	{	"path type",	kParamType_Integer,			0	},
	{	"item",			kParamType_InventoryObject,	1	},
};

static ParamInfo kParams_OneInventoryObject_OneInt[2] =
{
	{	"item",		kParamType_InventoryObject,	0	},
	{	"integer",	kParamType_Integer,			0	},
};

static ParamInfo kParams_OneFloat_OneOptionalInventoryObject[2] =
{
	{	"float",		kParamType_Float,			0	},
	{	"item",			kParamType_InventoryObject,	1	},
};

static ParamInfo kParams_OneMagicItem_OneOptionalInventoryObject[2] =
{
	{	"magic item",	kParamType_MagicItem,		0	},
	{	"item",			kParamType_InventoryObject,	1	},
};

static ParamInfo kParams_OneInventoryItem_OneOptionalInventoryObject[2] =
{
	{	"inv item",		kParamType_MagicItem,		0	},
	{	"target item",	kParamType_InventoryObject,	1	},
};

static ParamInfo kParams_OneFormList_OneOptionalInventoryObject[2] =
{
	{	"form list",	kParamType_FormList,		0	},
	{	"target item",	kParamType_InventoryObject,	1	},
};


static ParamInfo kParams_OneActorValue[1] =
{
	{	"actor value", kParamType_ActorValue, 0},
};

#define FORMAT_STRING_PARAMS 	\
	{"format string",	kParamType_String, 0}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1}, \
	{"variable",		kParamType_Float, 1} 

static ParamInfo kParams_FormatString[21] =
{
	FORMAT_STRING_PARAMS
};

#define SIZEOF_FMT_STRING_PARAMS 21
#define NUM_PARAMS(paramInfoName) SIZEOF_ARRAY(paramInfoName, ParamInfo)

static ParamInfo kParams_OneActorRef[1] =
{
	{	"actor reference",	kParamType_Actor,	0	},
};

static ParamInfo kParams_Axis[1] = 
{
	{	"axis",	kParamType_Axis,	0	},
};

static ParamInfo kParams_FormList[1] =
{
	{	"form list", kParamType_FormList,	0		},
};

static ParamInfo kParams_OneString_OneOptionalInventoryObject[2] =
{
	{	"string",		kParamType_String,			0	},
	{	"item",			kParamType_InventoryObject,	1	},
};

static ParamInfo kParams_TwoStrings[2] =
{
	{	"string",	kParamType_String,	0	},
	{	"string",	kParamType_String,	0	},
};

static ParamInfo kParams_OneObject[1] =
{
	{	"target item",	kParamType_TESObject,	0	},
};

static ParamInfo kParams_OneOptionalObject[1] =
{
	{	"target item",	kParamType_TESObject,	1	},
};

static ParamInfo kParams_OneString_OneOptionalObject[2] =
{
	{	"string",		kParamType_String,			0	},
	{	"item",			kParamType_TESObject,	1	},
};

static ParamInfo kParams_OneObject_OneOptionalObject[2] =
{
	{	"target item",	kParamType_TESObject,		0	},
	{	"object",		kParamType_TESObject,		1	},
};

static ParamInfo kParams_OneInt_OneOptionalObject[2] =
{
	{	"int",	kParamType_Integer,			0	},
	{	"item",			kParamType_TESObject,	1	},
};

static ParamInfo kParams_SetEquippedFloat[2] =
{
	{	"val", kParamType_Float, 0 },
	{	"slot", kParamType_Integer, 0 },
};

static ParamInfo kParams_FormListInteger[2] =
{
	{	"form list", kParamType_FormList,	0		},
	{	"index",	 kParamType_Integer,	0		}
};

static ParamInfo kParams_OneQuest[1] =
{
	{	"quest", kParamType_Quest, 0 }, 
};

static ParamInfo kParams_OneNPC[1] =
{
	{	"NPC",	kParamType_NPC,	1	},
};


static ParamInfo kParams_OneForm_OneFloat_OneForm_OneOptionalRank[4] =
{
	{	"item",		kParamType_AnyForm,		0	},
	{	"count",	kParamType_Float,		0	},
	{	"owner",	kParamType_AnyForm,		0	},
	{	"rank",		kParamType_Integer,		1	},
};
static ParamInfo kParams_OneSound_OneOptionalObjectRef[2] =
{

	{	"sound",	kParamType_AnyForm,		0	},
	{	"player",	kParamType_ObjectRef,	1	},
};

static ParamInfo kParams_OneWorldspace_OneInt[2] =
{
	{ "worldspace", kParamType_WorldSpace , 0},
	{ "flag",		kParamType_Integer,		0},
};

static ParamInfo kParams_OneWorldspace_TwoInts[3] =
{
	{ "worldspace", kParamType_WorldSpace , 0},
	{ "flag",		kParamType_Integer,		0},
	{ "flag",		kParamType_Integer,		0},
};

static ParamInfo kParams_OneFloat_OneFormatString[22] =
{
	{"float", kParamType_Float, 0},
	FORMAT_STRING_PARAMS
};

static ParamInfo kParams_OneString_OneForm[] =
{
	{	"string",	kParamType_String,	0	},
	{	"object",	kParamType_AnyForm,	0	},
};

static ParamInfo kParams_OneForm[] =
{
	{	"object",	kParamType_AnyForm,	0	},
};

static ParamInfo kParams_TwoForms[] =
{
	{	"object",	kParamType_AnyForm,	0	},
	{	"object",	kParamType_AnyForm,	0	},
};

static ParamInfo kParams_OneOptionalForm[] =
{
	{	"object",	kParamType_AnyForm,	1	},
};

static ParamInfo kParams_OneForm_OneOptionalForm[2] =
{
	{	"object",	kParamType_AnyForm,	0	},
	{	"object",	kParamType_AnyForm,	1	},
};

static ParamInfo kParams_TwoForms_OneOptionalFloat[3] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"form",	kParamType_AnyForm,	0	},
	{	"float",	kParamType_Float, 1	},
};

static ParamInfo kParams_OneForm_OneInt[2] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"int",	kParamType_Integer, 0	},
};

static ParamInfo kParams_OneForm_OneInt_OneString[3] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"int",	kParamType_Integer, 0	},
	{	"string",	kParamType_String,	0	},
};

static ParamInfo kParams_OneForm_OneInt_OneOptionalForm[3] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"int",	kParamType_Integer, 0	},
	{	"object",	kParamType_AnyForm,	1	},
};

static ParamInfo kParams_OneForm_TwoInts[3] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"int",	kParamType_Integer, 0	},
	{	"int",	kParamType_Integer, 0	},
};
;
static ParamInfo kParams_OneAxis_OneFloat[2] =
{
	{"axis", kParamType_Axis, 0},
	{"float", kParamType_Float, 0}
};


static ParamInfo kParams_OneForm_ThreeInts[4] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"int",	kParamType_Integer, 0	},
	{	"int",	kParamType_Integer, 0	},
	{	"int",	kParamType_Integer, 0	},
};

static ParamInfo kParams_OneForm_ThreeFloats[4] =
{
	{	"form",	kParamType_AnyForm,	0	},
	{	"float",	kParamType_Float, 0	},
	{	"float",	kParamType_Float, 0	},
	{	"float",	kParamType_Float, 0	},
};


static ParamInfo kParams_OneForm_OneInt_OneFloat[3] =
{
	{ "Form", kParamType_AnyForm, 0 },
	{ "Integer", kParamType_Integer, 0 },
	{ "Float", kParamType_Float, 0 }
};

static ParamInfo kParams_OneString_OneInt_OneForm[3] =
{
	{ "string",	kParamType_String,	0 },
	{ "int",	kParamType_Integer, 0 },
	{ "form",	kParamType_AnyForm,	0 },
};

static ParamInfo kParams_OneForm_OneString[2] =
{
	{ "Form", kParamType_AnyForm, 0 },
	{ "String", kParamType_String, 0 },
};

static ParamInfo kParams_SetHotkeyItem[2] =
{
	{	"hotkey",	kParamType_Integer,		0	},
	{	"item",		kParamType_AnyForm,		0	},
};

static ParamInfo kParams_OneOptionalAxis[1] =
{
	{ "axis", kParamType_Axis, 1}
};

static ParamInfo kParams_OneMessage[1] =
{
	{"message", kParamType_Message, 0}
};

static ParamInfo kParams_OneMessage_OneInt[2] =
{

	{"message", kParamType_Message, 0},
	{"int",	kParamType_Integer,		0 }
};