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


// ��������� ����� �� ����� �� �����
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2);


// ����� ��������� ����� ���� ������������ ��� ��������
POINT GetLocalCoordinates(HWND hWnd);
RECT* GetLocalRect(HWND hWnd);


// �������� �� int �� ���� ��� ��� ����������
BOOL is_int(string str);


// ���������� ������ ������������� ��������� ������������ 100% ���������� ��� ��� � �����������
// ������� ��� ������� ����� ����������� ����� � ����������
void intersectionPoints(double a, double b, double r, double A, double B, double C, double& x1, double& y1, double& x2, double& y2);


// ������� ������� ������������� ������ a*x + b*y + c = 0
void calculateLineCoefficients(double x1, double y1, double x2, double y2, double& a, double& b, double& c);