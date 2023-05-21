#pragma once

#include "Lib.h"

#define FLAG_DEFAULT_SIZE V3(80, 30, 3)
#define FLAG_DEFAULT_BDC V3(102, 102, 102)
#define FLAG_DEFAULT_BHC V3(112, 112, 112)
#define FLAG_DEFAULT_BFC V3(255, 255, 255)
#define FLAG_DEFAULT_BKGND V3(41, 41, 41)
#define FLAG_DEFAULT_BKGNDH V3(48, 48, 48)
#define FLAG_DEFAULT_BKGNDF V3(56, 56, 56)
#define FLAG_DEFAULT_TYPECOL V3(255, 255, 255)
#define FLAG_DEFAULT_UNFOC_TYPECOL V3(220, 220, 220)
#define FLAG_DEFAULT_UNFOC_TEXTCOL V3(153, 153, 153)

#define REDRAW_IDT 1
#define REDRAW_RATE 1000 / 144

// structure for some stuff (aka  Œ—“€À‹)
struct FLAGPARAMS
{
	UINT bdWidth = 3;
	V3 bdDefCol = FLAG_DEFAULT_BDC;
	V3 bdFocCol = FLAG_DEFAULT_BFC;
	V3 bdHovCol = FLAG_DEFAULT_BHC;
	V3 bkCol = FLAG_DEFAULT_BKGND;
	V3 bkHovCol = FLAG_DEFAULT_BKGNDH;
	V3 bkFocCol = FLAG_DEFAULT_BKGNDF;
	V3 typeCol = FLAG_DEFAULT_TYPECOL;
	V3 unfocTypeCol = FLAG_DEFAULT_UNFOC_TYPECOL;
	V3 unfocTextCol = FLAG_DEFAULT_UNFOC_TEXTCOL;
};
#define FLAG_DEFAULT_PARAMS FLAGPARAMS{}

class Flag : Control<Flag, FLAGPARAMS>
{
private:
	// private variables
	string unfoctext;

	// private flags
	enum states { inactive, hovered, focused };
	states state;

	// private methods
	void CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void Redraw();
	HWND GenWnd(HWND _hParWnd);

	// private static methods
	static void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);

public:
	BOOL is = 0;

	//public methods
	Flag(
		HWND _hParWnd,
		string _UnfocusedText,
		V3 _Position,
		V3 _Size = FLAG_DEFAULT_SIZE,
		FLAGPARAMS _Params = FLAG_DEFAULT_PARAMS
	);

	// public static methods
	static LRESULT CALLBACK FlagProc(HWND, UINT, WPARAM, LPARAM);
protected:

};

typedef Flag FLAG;