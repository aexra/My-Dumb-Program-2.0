#pragma once

#include "Header.h"

#ifndef FIELD_H
#define FIELD_H

/* Window class */
#define FIELD_WC  _T("FieldControl")

#endif  /* FIELD_H */

class Field {
private:
	static LRESULT CALLBACK FieldWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	RECT rect;
	HWND hWnd;
public:
	Field(HWND _hWnd);

	void SetWindow(HWND& _hWnd);
	BOOL IsPtInBorders(POINT _pt);
	BOOL CheckVerticeCollisions(POINT _pt);
	RECT GetRect();
	RECT SetRect(RECT _rect);

	static void FieldRegister(void);
	static void FieldUnregister(void);
};