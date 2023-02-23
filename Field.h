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
public:
	Field();
	static void FieldRegister(void);
	static void FieldUnregister(void);
};