#include "GameTypes.h"
#include "GameSettings.h"


GameSettingCollection ** g_GameSettingCollection = (GameSettingCollection**)0x010701A8;
IniSettingCollection ** g_IniSettingCollection = (IniSettingCollection**)0x0116D6F4;
IniSettingCollection ** g_IniPrefCollection = (IniSettingCollection**)0x01179578;


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

UInt32 Setting::GetType()
{
	if (!name) return kSetting_Other;

	switch (name[0])
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

bool Setting::GetNumeric(double& out)
{
	switch (GetType())
	{
	case kSetting_Bool:
	case kSetting_Integer:
		out = data.i;
		return true;
	case kSetting_Unsigned:
		out = data.u;
		return true;
	case kSetting_Float:
		out = data.f;
		return true;
	default:
		return false;
	}
	
}

bool Setting::SetNumeric(double newVal)
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
		data.u = newVal;
		return true;
	case kSetting_Float:
		data.f = newVal;
		return true;
	default:
		return false;
	}
}

class IniSettingFinder
{
public:
	const char* m_settingName;
	IniSettingFinder(const char* name) : m_settingName(name)
	{
	}
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

		if (coll && !coll->settings.Empty()) {
			setting = coll->settings.Find(finder);
		}

	if (!setting)
	{
		coll = IniSettingCollection::GetIniSettings();
		setting = coll->settings.Find(finder);
	}

	if (!setting) {
		coll = RendererSettingCollection::GetCollection();
		setting = coll->settings.Find(finder);
	}

	if (setting)
	{
		*out = setting;
		return true;
	}

	return false;
}