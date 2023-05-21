#pragma once

#include "Lib.h"
#include "ThemeManager.h"

#define STATIC_DEFAULT_SIZE V3(80, 30, 3)
#define STATIC_DEFAULT_BORDER V3(102, 102, 102)
#define STATIC_DEFAULT_BKGND V3(41, 41, 41)
#define STATIC_DEFAULT_TEXTCOL V3(255, 255, 255)

#define REDRAW_IDT 1
#define REDRAW_RATE 1000 / 144

// structure for some stuff (aka  Œ—“€À‹)
struct STATICPARAMS
{
	UINT bdWidth = 3;
	aligns alignv = aligns::center;
	aligns alignh = aligns::center;
	INT charHeight = 0;
	V3 bdDefCol = STATIC_DEFAULT_BORDER;
	V3 bkCol = STATIC_DEFAULT_BKGND;
	V3 textCol = STATIC_DEFAULT_TEXTCOL;
	wstring font = L"Comic Sans MS";
};
#define STATIC_DEFAULT_PARAMS STATICPARAMS{}

class Static : public Control<Static, STATICPARAMS>
{
private:
	// private variables
	HWND placeholder;
	UINT_PTR nIDEvent;
	HBRUSH hPlaceholderBKBrush;

	// private flags
	states state;
	states laststate;

	// private methods
	void CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void Redraw();
	void GenWnd(HWND _hParWnd);
	void UpdateAligning();

	// private static methods
	static void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);

public:

	//public methods
	Static(
		HWND _hParWnd,
		string _Text,
		V3 _Position,
		UINT_PTR _nIDEvent = NULL,
		V3 _Size = STATIC_DEFAULT_SIZE,
		STATICPARAMS _Params = STATIC_DEFAULT_PARAMS
	);
	BOOL MoveControl(V3 _To);
	string SetText(string _Text);
	string AddText(string _Text);
	string AddLine(string _Text);
	void Disable();
	void Enable();
	void Show();
	void Hide();
	void SetAlignV(aligns _Al);
	void SetAlignH(aligns _Al);

	// public static methods
	static LRESULT CALLBACK StaticProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK PlaceholderProc(HWND, UINT, WPARAM, LPARAM);
protected:

};

typedef Static STATIC;