#pragma once

#include "Header.h"
#include "Vector3.h"

struct PALETTE
{
	V3 bk;
	V3 bd;
	V3 text;
	V3 fbk;
	V3 fbd;
	V3 link;
	V3 vbk;
	V3 vbd;
	V3 selection;
}
const dark{
	V3(20, 20, 20),
	V3(36, 35, 35),
	V3(255, 255, 255),
	V3(33, 31, 31),
	V3(46, 43, 43),
	V3(135, 127, 127),
	V3(64, 61, 61),
	V3(200, 200, 200),
	V3(168, 168, 13)
},
const light{

}
;

class ThemeManager
{
private:
	static ThemeManager* p_instance;
	BOOL theme = 0; // 0 = dark, 1 = light
	PALETTE palette;
	ThemeManager();
public:
	static ThemeManager* GetInstance();
	void SetLight();
	void SetDark();
	BOOL GetTheme();
	PALETTE GetPalette();
};

