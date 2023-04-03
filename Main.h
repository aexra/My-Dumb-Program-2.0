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


// ������� ������� ������������� ������ a*x + b*y + c = 0
void calculateLineCoefficients(POINT _Pt1, POINT _Pt2, double& _A, double& _B, double& _C);


// ���������� ������ ������������� ��������� ������������ 100% ���������� ��� ��� � �����������
// ������� ��� ������� ����� ����������� ������ � ����������
void intersectionPoints(POINT _CirPT, double r, double A, double B, double C, POINT& _Pt1, POINT& _Pt2);


// ���������� ������ ������������� ��������� ������������ 100% ���������� ��� ��� � �����������
// ������� ��� ������� ����� ����������� ������� � ����������
vector<POINT> intersectionPoints(POINT _PtA, POINT _PtB, POINT _PtC, double R);


// ���������� ����� ��������
double PointDistance(const POINT& _Pt1, const POINT& _Pt2);