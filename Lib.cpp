#include "Lib.h"

RECT GetLocalRect(HWND hWnd)
{
	RECT childr;
	GetWindowRect(hWnd, &childr);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&childr, 2);
	return childr;
}
VOID GetLocalRect(HWND hWnd, LPRECT r)
{
	GetWindowRect(hWnd, r);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)r, 2);
}
WNDCLASS NewWindowClass(LPCWSTR Name, WNDPROC Procedure, HBRUSH BGColor, HCURSOR Cursor, HICON Icon) {
	WNDCLASS NWC = { };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}
void InitLib()
{
	

	WNDCLASS wc = { };
	RegisterClass(&(wc = NewWindowClass(EDIT_WC, Edit::EditProc, (HBRUSH)GetStockObject(HOLLOW_BRUSH))));
	RegisterClass(&(wc = NewWindowClass(BUTTON_WC, Button::ButtonProc, (HBRUSH)GetStockObject(HOLLOW_BRUSH))));
	RegisterClass(&(wc = NewWindowClass(STATIC_WC, Static::StaticProc, (HBRUSH)GetStockObject(HOLLOW_BRUSH))));
	RegisterClass(&(wc = NewWindowClass(VBOX_WC, NULL)));
	RegisterClass(&(wc = NewWindowClass(HBOX_WC, NULL)));
}
BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	// т.к. WinAPI не предлагает функцию для рисования линии от
	// точки до точки, а LineTo принимает конечную позицию от текущей,
	// заданной MoveToEx(), напишем свою функцию:
	MoveToEx(hdc, x1, y1, NULL);
	return LineTo(hdc, x2, y2);
}

POINT GetLocalCoordinates(HWND hWnd)
{
	RECT Rect;
	POINT pt;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	pt.x = Rect.left;
	pt.y = Rect.top;
	return pt;
}
BOOL is_int(string str)
{
	if (str.length() < 1) return false;
	if (str.length() > 9) return false;

	if (count(str.begin(), str.end(), '-') > 0 && str[0] != '-') return false;
	if (count(str.begin(), str.end(), '-') > 1) return false;

	for (char c : str) {
		if (!('0' <= c && c <= '9' || c == '-')) return false;
	}

	return true;
}
void intersectionPoints(POINT _CirPT, double r, double A, double B, double C, POINT& _Pt1, POINT& _Pt2)
{
	double discriminant = pow(A * B * (C - _CirPT.x), 2) - (pow(A, 2) + pow(B, 2)) * (pow(C, 2) + pow(_CirPT.y, 2) - pow(r, 2));
	if (discriminant >= 0) {
		_Pt1.x = (-A * B * (C - _CirPT.x) + sqrt(discriminant)) / (pow(A, 2) + pow(B, 2));
		_Pt2.y = (-A * B * (C - _CirPT.x) - sqrt(discriminant)) / (pow(A, 2) + pow(B, 2));
		_Pt1.x = -(B * _Pt1.y + C) / A;
		_Pt2.x = -(B * _Pt2.y + C) / A;
	}
	else {
		_Pt1.x = _Pt2.x = _Pt1.y = _Pt2.y = NULL;
	}
}
vector<POINT> intersectionPoints(POINT _PtA, POINT _PtB, POINT _PtC, double R)
{
	vector<POINT> intersections;
	double dx = _PtB.x - _PtA.x;
	double dy = _PtB.y - _PtA.y;
	double A = dx * dx + dy * dy;
	double B = 2 * (dx * (_PtA.x - _PtC.x) + dy * (_PtA.y - _PtC.y));
	double C = (_PtA.x - _PtC.x) * (_PtA.x - _PtC.x) + (_PtA.y - _PtC.y) * (_PtA.y - _PtC.y) - R * R;
	double D = B * B - 4 * A * C;
	if (D >= 0) {
		double t1 = (-B + sqrt(D)) / (2 * A);
		double t2 = (-B - sqrt(D)) / (2 * A);
		if (t1 >= 0 && t1 <= 1) {
			POINT pt = { };
			pt.x = _PtA.x + t1 * dx;
			pt.y = _PtA.y + t1 * dy;
			intersections.push_back(pt);
		}
		if (D > 0 && t2 >= 0 && t2 <= 1) {
			POINT pt = { };
			pt.x = _PtA.x + t2 * dx;
			pt.y = _PtA.y + t2 * dy;
			intersections.push_back(pt);
		}
	}
	return intersections;
}
void calculateLineCoefficients(POINT _Pt1, POINT _Pt2, double& _A, double& _B, double& _C)
{
	_A = _Pt2.y - _Pt1.y;
	_B = _Pt1.x - _Pt2.x;
	_C = _Pt2.x * _Pt1.y - _Pt1.x * _Pt2.y;
}
double PointDistance(const POINT& _Pt1, const POINT& _Pt2)
{
	return sqrt(pow(_Pt1.x - _Pt2.x, 2) + pow(_Pt1.y - _Pt2.y, 2));
}
