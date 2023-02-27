#pragma once

#include <Windows.h>
#include <Windowsx.h>
#include <Winuser.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <tchar.h>
#include <shellapi.h>

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


// timers
constexpr UINT VERTICE_REFRESH_IDT = 256;


// refresh rates
constexpr UINT VERTICE_REFRESH_RATE = 1000 / 60;