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