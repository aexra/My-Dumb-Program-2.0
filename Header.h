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
#define minVerticeID 101
#define maxVerticeID 255

// windows' IDs
#define OnInfoClicked 1
#define OnExitClicked 2
#define OnSaveClicked 3
#define OnLoadGraphClicked 4
#define FieldID 5
#define OnDeleteVerticeClicked 6
#define OnGitSourceClicked 7
#define OnSaveAsClicked 8
#define OnNewClicked 9
#define OnLightThemeClicked 10
#define OnDarkThemeClicked 11
#define InspectorStatic 12
#define GraphName 13
#define IsOriented 14
#define IsWeighted 15
#define VerticeName 16
#define TransformPosition 17
#define Weight 18


// vertice constants
#define VERTICE_REFRESH_IDT 256
#define VERTICE_REFRESH_RATE 1000 / 60
#define MAX_VERTICE 30
#define VERTICE_DISTANCE_ERROR 17