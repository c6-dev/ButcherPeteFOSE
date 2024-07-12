#include "GameTypes.h"
#include "GameSettings.h"


GameSettingCollection ** g_GameSettingCollection = (GameSettingCollection**)0x010701A8;
IniSettingCollection ** g_IniSettingCollection = (IniSettingCollection**)0x0116D6F4;
IniSettingCollection ** g_IniPrefCollection = (IniSettingCollection**)0x01179578;


UInt32 Setting::GetType()
{
	if (!name) return kSetting_Other;

	switch(name[0]) 
	{
		case 'b': return kSetting_Bool;
		case 'c': return kSetting_c;
		case 'i': return kSetting_Integer;
		case 'u': return kSetting_Unsigned;
		case 'f': return kSetting_Float;
		case 's':
		case 'S':
			return kSetting_String;
		case 'r': return kSetting_r;
		case 'a': return kSetting_a;
		default:
			return kSetting_Other;
	}
}

bool Setting::Get(double& out)
{
	switch (GetType())
	{
	case kSetting_Bool:
	case kSetting_Integer:
		out = data.i;
		return true;
	case kSetting_Unsigned:
		out = data.uint;
		return true;
	case kSetting_Float:
		out = data.f;
		return true;
	default:
		return false;
	}
}

bool Setting::Set(double newVal)
{
	switch (GetType())
	{
	case kSetting_Bool:
		data.i = newVal ? 1 : 0;
		return true;
	case kSetting_Integer:
		data.i = newVal;
		return true;
	case kSetting_Unsigned:
		data.uint = newVal;
		return true;
	case kSetting_Float:
		data.f = newVal;
		return true;
	default:
		return false;
	}
}

bool GameSettingCollection::GetGameSetting(char* settingName, Setting** out)
{
	NiTMapBase <const char*, Setting*>* map = &settingMap;
	out = &map->Get(settingName)->data;
	return true;
}

GameSettingCollection * GameSettingCollection::GetSingleton()
{
	return *g_GameSettingCollection;
}

IniSettingCollection* IniSettingCollection::GetIniSettings()
{
	return *g_IniSettingCollection;
}

IniSettingCollection* IniSettingCollection::GetIniPrefs()
{
	return *g_IniPrefCollection;
}

class IniSettingFinder
{
public:
	const char* m_settingName;
	IniSettingFinder(const char* name) : m_settingName(name)
		{	}
	bool Accept(Setting* info)
	{
		if (!_stricmp(m_settingName, info->name))
			return true;
		else
			return false;
	}
};

bool GetIniSetting(const char* settingName, Setting** out)
{
	Setting* setting = NULL;
	IniSettingFinder finder(settingName);

	// check prefs first
	IniSettingCollection* coll = IniSettingCollection::GetIniPrefs();
	if (coll)
		setting = coll->settings.Find(finder);

	if (!setting)
	{
		coll = IniSettingCollection::GetIniSettings();
		setting = coll->settings.Find(finder);
	}

	if (setting)
	{
		*out = setting;
		return true;
	}

	return false;
}
