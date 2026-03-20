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