#pragma once

#include "Header.h"


// ������� ��������� �������� ����
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);


// �������� ���� � �����������
WNDCLASS NewWindowClass(HBRUSH, HCURSOR, HINSTANCE, HICON, LPCWSTR, WNDPROC);


// ��������� ���. ������ (�� ���������)
//DWORD WINAPI LineDrawerThreadProc(LPVOID lParam);


// ���������� ���� ��� �������� ����
void MainWndAddMenus(HWND);

// ���������� �������� ��� �������� ����
void MainWndAddWidgets(HWND);