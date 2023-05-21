#include "ThemeManager.h"

ThemeManager* ThemeManager::p_instance = nullptr;

ThemeManager::ThemeManager()
{

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
}

void ThemeManager::SetDark()
{
	palette = dark;
}

BOOL ThemeManager::GetTheme()
{
	return theme;
}

PALETTE ThemeManager::GetPalette()
{
	return palette;
}


