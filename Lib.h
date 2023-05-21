#pragma once

#define vRGB(v) RGB(v.x, v.y, v.z)

enum states { hovered, pressed, enabled, disabled, valid, invalid };
enum class aligns { left, center, right, top, bottom};

//#define LIGHT_THEME

#ifndef LIGHT_THEME
#define MAIN_BK_COL V3(30, 30, 30)
#else
#define MAIN_BK_COL V3(230, 230, 230)
#endif

#define EDIT_WC L"MyCustomEditControl"
#define FLAG_WC L"MyCustomFlagControl"
#define BUTTON_WC L"MyCustomButtonControl"
#define STATIC_WC L"MyCustomStaticControl"
#define VBOX_WC L"MyCustomVBoxControl"
#define HBOX_WC L"MyCustomHBoxControl"

#include "Vector2.h"
#include "Vector3.h"
#include "Transform.h"
#include "Control.h"
#include "Edit.h"
#include "Flag.h"
#include "Button.h"
#include "Static.h"


// Возвращает размеры окна hWnd
RECT GetLocalRect(HWND hWnd);


// Изменяет r на значение размеров окна hWnd
VOID GetLocalRect(HWND hWnd, LPRECT r);


// Коротенькая простенькая функция чтобы было удобно
WNDCLASS NewWindowClass(
	LPCWSTR Name,
	WNDPROC Procedure,
	HBRUSH BGColor = (HBRUSH)COLOR_WINDOW,
	HCURSOR Cursor = LoadCursor(NULL, IDC_ARROW),
	HICON Icon = LoadIcon(NULL, IDI_QUESTION)
);


// Инициализация библиотеки (рагистрация окон, прочая ...)
void InitLib();


// Рисование линии от точки до точки
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);


// Поиск координат точки окна относительно его родителя
POINT GetLocalCoordinates(HWND hWnd);


// Проверка на int из моих лаб Аси Михайловны
BOOL is_int(string str);


// Функция расчета коэффициентов прямой a*x + b*y + c = 0
void calculateLineCoefficients(POINT _Pt1, POINT _Pt2, double& _A, double& _B, double& _C);


// Нереальная лучшая гениальнейшая авторская оригинальная 100% бесплатная без смс и регистрации
// функция для расчета точек пересечения прямой и окружности
void intersectionPoints(POINT _CirPT, double r, double A, double B, double C, POINT& _Pt1, POINT& _Pt2);


// Нереальная лучшая гениальнейшая авторская оригинальная 100% бесплатная без смс и регистрации
// функция для расчета точек пересечения отрезка и окружности
vector<POINT> intersectionPoints(POINT _PtA, POINT _PtB, POINT _PtC, double R);


// Расстояние между поинтами
double PointDistance(const POINT& _Pt1, const POINT& _Pt2);