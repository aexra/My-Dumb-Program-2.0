#pragma once

#include "Header.h"
#include "Transform.h"

template <class T, class PARAMS>
class Control
{
private:
public:
	static map<HWND, T*> objmap;
	TRANSFORM transform;
	PARAMS params;

	HWND GetWindow()
	{
		return wnd;
	}
	HWND SetWindow(HWND hWnd)
	{
		return wnd = hWnd;
	}
	string SetText(string _Text)
	{
		SetWindowTextA(wnd, _Text.c_str());
		return text = _Text;
	}
	string GetText()
	{
		return text;
	}
	BOOL MoveControl(V3 _To)
	{
		transform.Translate(_To);
		return MoveWindow(wnd, _To.x, _To.y, transform.size.x, transform.size.y, TRUE);
	}
	BOOL Invalidate(void)
	{
		return InvalidateRect(wnd, NULL, FALSE);
	}
protected:
	HWND wnd;
	string text;
};
