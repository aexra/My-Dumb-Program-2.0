#pragma once

#include "Header.h"


LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
WNDCLASS NewWindowClass(HBRUSH, HCURSOR, HINSTANCE, HICON, LPCWSTR, WNDPROC);

DWORD WINAPI Thread2(LPVOID lParam);

void MainWndAddMenus(HWND);
void MainWndAddWidgets(HWND);

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);

POINT GetLocalCoordinates(HWND hWnd);
RECT* GetLocalRect(HWND hWnd);
