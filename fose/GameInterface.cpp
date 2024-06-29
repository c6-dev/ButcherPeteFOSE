#include "GameInterface.h"
#include "GameTypes.h"



InterfaceManager** g_interfaceManager = (InterfaceManager**)0x01075B24;
NiTArray<TileMenu*> * g_TileMenuArray = (NiTArray<TileMenu*> *)0x0106A7BC;
bool * g_MenuVisibilityArray = (bool *)0x011793DB;	// size 3C
const _QueueUIMessage QueueUIMessage = (_QueueUIMessage)0x0061B850;


InterfaceManager * InterfaceManager::GetSingleton(void)
{
	return *g_interfaceManager;
}

bool InterfaceManager::IsMenuVisible(UInt32 menuType)
{
	if (menuType >= kMenuType_Message)
		menuType -= kMenuType_Message;

	if (menuType < 0x3C)
		return g_MenuVisibilityArray[menuType];

	return false;
}

Menu* InterfaceManager::GetMenuByType(UInt32 menuType)
{
	if (menuType >= kMenuType_Message)
		menuType -= kMenuType_Message;

	if (menuType < 0x3C)
	{
		TileMenu* tileMenu = g_TileMenuArray->Get(menuType);
		if (tileMenu)
			return tileMenu->menu;
	}

	return NULL;
}

Tile::Value* InterfaceManager::GetMenuComponentValue(char* componentPath)
{
	// path is of format "MenuType/tile/tile/.../traitName" following hierarchy defined in menu's xml

	// get menu name - stored by game as "&MENUNAME;" so need to fix it up
	/*char* slashPos = strpbrk(componentPath, "\\/");
	if (slashPos)
	{
		*slashPos = 0;
		char menuName[0x20];
		UInt32 len = strlen(componentPath);
		strcpy_s(menuName + 1, len + 1, componentPath);
		menuName[len + 1] = ';';
		menuName[0] = '&';
		menuName[len + 2] = '\0';

		UInt32 menuID = Tile::TraitNameToID(menuName);
		if (menuID >= kMenuType_Min && menuID <= kMenuType_Max)
		{
			Menu* menu = GetMenuByType(menuID);
			if (menu)
				return menu->rootTile->GetComponentValue(slashPos + 1);
		}
	}*/

	return NULL;
}
