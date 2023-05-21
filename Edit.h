#pragma once

#include "Lib.h"

#define EDIT_DEFAULT_SIZE V3(80, 30, 3)
#define EDIT_DEFAULT_BDC V3(102, 102, 102)
#define EDIT_DEFAULT_BHC V3(112, 112, 112)
#define EDIT_DEFAULT_BFC V3(0, 128, 0)
#define EDIT_DEFAULT_BKGND V3(41, 41, 41)
#define EDIT_DEFAULT_BKGNDH V3(48, 48, 48)
#define EDIT_DEFAULT_BKGNDF V3(56, 56, 56)
#define EDIT_DEFAULT_TYPECOL V3(255, 255, 255)
#define EDIT_DEFAULT_UNFOC_TYPECOL V3(220, 220, 220)
#define EDIT_DEFAULT_UNFOC_TEXTCOL V3(153, 153, 153)

#define REDRAW_IDT 1
#define REDRAW_RATE 1000 / 144

// structure for some stuff (aka  Œ—“€À‹)
struct EDITPARAMS
{
	UINT bdWidth = 3;
	V3 bdDefCol = EDIT_DEFAULT_BDC;
	V3 bdFocCol = EDIT_DEFAULT_BFC;
	V3 bdHovCol = EDIT_DEFAULT_BHC;
	V3 bkCol = EDIT_DEFAULT_BKGND;
	V3 bkHovCol = EDIT_DEFAULT_BKGNDH;
	V3 bkFocCol = EDIT_DEFAULT_BKGNDF;
	V3 typeCol = EDIT_DEFAULT_TYPECOL;
	V3 unfocTypeCol = EDIT_DEFAULT_UNFOC_TYPECOL;
	V3 unfocTextCol = EDIT_DEFAULT_UNFOC_TEXTCOL;
};
#define EDIT_DEFAULT_PARAMS EDITPARAMS{}

class Edit : Control<Edit, EDITPARAMS>
{
private:
	// private variables
	string unfoctext;

	// private flags
	enum states {inactive, hovered, focused};
	states state;

	// private methods
	void CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void Redraw();
	HWND GenWnd(HWND _hParWnd);

	// private static methods
	static void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);

public:

	//public methods
	Edit(
		HWND _hParWnd,
		string _UnfocusedText,
		V3 _Position,
		V3 _Size = EDIT_DEFAULT_SIZE,
		EDITPARAMS _Params = EDIT_DEFAULT_PARAMS
	);
	
	// public static methods
	static LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);
protected:

};

typedef Edit EDIT;