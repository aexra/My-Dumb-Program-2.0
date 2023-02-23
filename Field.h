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

	HWND hWnd;
public:
	Field(HWND _hWnd);

	void SetWindow(HWND& _hWnd);

	static void FieldRegister(void);
	static void FieldUnregister(void);
};