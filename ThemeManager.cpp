#include "ThemeManager.h"
#include "Lib.h"

ThemeManager* ThemeManager::p_instance = nullptr;
extern HBRUSH fbkb;

ThemeManager::ThemeManager()
{
	palette = (theme == 0 ? dark : light);
}

ThemeManager* ThemeManager::GetInstance()
{
	if (p_instance)
		return p_instance;
	return (p_instance = new ThemeManager());
}

void ThemeManager::SetLight()
{
	palette = light;
	DeleteObject(fbkb);
	fbkb = CreateSolidBrush(vRGB(dark.fbk));
}

void ThemeManager::SetDark()
{
	palette = dark;
	DeleteObject(fbkb);
	fbkb = CreateSolidBrush(vRGB(light.fbk));
}

BOOL ThemeManager::GetTheme()
{
	return theme;
}

PALETTE ThemeManager::GetPalette()
{
	return palette;
}


