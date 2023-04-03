#pragma once

#include "Header.h"


// Оконная процедура главного окна
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


// Создание окна с параметрами
WNDCLASS NewWindowClass(HBRUSH, HCURSOR, HINSTANCE, HICON, LPCWSTR, WNDPROC);


// Процедура доп. потока (не акутально)
//DWORD WINAPI LineDrawerThreadProc(LPVOID lParam);


// Добавление меню для главного окна
void MainWndAddMenus(HWND);

// Добавление виджетов для главного окна
void MainWndAddWidgets(HWND);


// Рисование линии от точки до точки
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);


// Поиск координат точки окна относительно его родителя
POINT GetLocalCoordinates(HWND hWnd);
RECT* GetLocalRect(HWND hWnd);


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