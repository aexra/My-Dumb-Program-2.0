#pragma once

#include "Lib.h"

#define BUTTON_DEFAULT_SIZE V3(80, 30, 3)
#define BUTTON_DEFAULT_BDC V3(102, 102, 102)
#define BUTTON_DEFAULT_BHC V3(112, 112, 112)
#define BUTTON_DEFAULT_BPC V3(0, 128, 0)
#define BUTTON_DEFAULT_BIC V3(204, 0, 0)
#define BUTTON_DEFAULT_BKGND V3(41, 41, 41)
#define BUTTON_DEFAULT_BKGNDH V3(48, 48, 48)
#define BUTTON_DEFAULT_BKGNDP V3(56, 56, 56)
#define BUTTON_DEFAULT_TEXTCOL V3(255, 255, 255)
#define BUTTON_DEFAULT_HOVERED_TEXTCOL V3(220, 220, 220)
#define BUTTON_DEFAULT_PRESSED_TEXTCOL V3(153, 153, 153)
// DISABLED MODE
#define BUTTON_DEFAULT_DISABLED_BK V3(26, 26, 26);
#define BUTTON_DEFAULT_DISABLED_BD V3(87, 87, 87);
#define BUTTON_DEFAULT_DISABLED_TEXT V3(230, 230, 230);

#define REDRAW_IDT 1
#define REDRAW_RATE 1000 / 144
#define UNPRESS_IDT 2
#define UNPRESS_DURATION 1000 / 144

// structure for some stuff (aka  Œ—“€À‹)
struct BUTTONPARAMS
{
	UINT bdWidth = 3;
	aligns alignv = aligns::center;
	aligns alignh = aligns::center;
	states startstate = enabled;
	INT charHeight = 0;
	V3 bdDefCol = BUTTON_DEFAULT_BDC;
	V3 bdHovCol = BUTTON_DEFAULT_BDC + 60;
	V3 bdPreCol = BUTTON_DEFAULT_BDC + 100;
	V3 bdInvCol = BUTTON_DEFAULT_BIC;
	V3 bdDisCol = BUTTON_DEFAULT_DISABLED_BD;
	V3 bkCol = BUTTON_DEFAULT_BKGND;
	V3 bkHovCol = BUTTON_DEFAULT_BKGND;
	V3 bkPreCol = BUTTON_DEFAULT_BKGND;
	V3 bkDisCol = BUTTON_DEFAULT_DISABLED_BK;
	V3 textCol = BUTTON_DEFAULT_TEXTCOL;
	V3 textHovCol = BUTTON_DEFAULT_TEXTCOL;
	V3 textPreCol = BUTTON_DEFAULT_TEXTCOL;
	V3 textDisCol = BUTTON_DEFAULT_DISABLED_TEXT;
};
#define BUTTON_DEFAULT_PARAMS BUTTONPARAMS{}

class Button : public Control<Button, BUTTONPARAMS>
{
private:
	// private variables
	UINT_PTR nIDEvent;
	HWND placeholder;
	HBRUSH hPlaceholderBKBrush;

	// private flags
	states state;
	states laststate;

	// private methods
	void CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void Redraw();
	void GenWnd(HWND _hParWnd);
	void Press();

	// private static methods
	static void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);

public:

	//public methods
	Button(
		HWND _hParWnd,
		string _Text,
		V3 _Position,
		UINT_PTR _nIDEvent,
		V3 _Size = BUTTON_DEFAULT_SIZE,
		BUTTONPARAMS _Params = BUTTON_DEFAULT_PARAMS
	);
	//BOOL MoveControl(V3);
	void Disable();
	void Enable();
	void Show();
	void Hide();
	BOOL IsDisabled();

	// public static methods
	static LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK PlaceholderProc(HWND, UINT, WPARAM, LPARAM);
	static void DeleteButton(Button* btn);
protected:

};

typedef Button BUTTON;