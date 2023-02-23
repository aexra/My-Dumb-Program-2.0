#pragma once

#include "Header.h"
using namespace std;

// borders of vertices' IDs
constexpr UINT minVerticeID = 101;
constexpr UINT maxVerticeID = 255;

// windows' IDs
constexpr UINT OnInfoClicked = 1;
constexpr UINT OnExitClicked = 2;
constexpr UINT OnSaveClicked = 3;
constexpr UINT OnLoadGraphClicked = 4;
constexpr UINT FieldID = 5;
constexpr UINT OnDeleteVerticeClicked = 6;
constexpr UINT OnGitSourceClicked = 7;
constexpr UINT OnSaveAsClicked = 8;
constexpr UINT OnNewClicked = 9;
constexpr UINT OnLightThemeClicked = 10;
constexpr UINT OnDarkThemeClicked = 11;
constexpr UINT InspectorStatic = 12;
constexpr UINT GraphName = 13;
constexpr UINT IsOriented = 14;
constexpr UINT IsWeighted = 15;
constexpr UINT VerticeName = 16;
constexpr UINT TransformPosition = 17;
constexpr UINT Weight = 18;



LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
WNDCLASS NewWindowClass(HBRUSH, HCURSOR, HINSTANCE, HICON, LPCWSTR, WNDPROC);

void MainWndAddMenus(HWND);
void MainWndAddWidgets(HWND);

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);

UINT GenerateNewVerticeID();

RECT GetLocalCoordinates(HWND hWnd);
