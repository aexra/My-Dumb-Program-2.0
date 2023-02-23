#include "Main.h"
#include "Field.h"
#include "Vertice.h"





/* GLOBAL VARIABLES */


// FONTS
HFONT titlef = CreateFontA(24, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "Comic Sans MS");
HFONT textf = CreateFontA(21, 9, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "Calibri");


// BRUSHES
HBRUSH SysGrey = (HBRUSH)COLOR_WINDOW;
HBRUSH TitleBrush = CreateSolidBrush(RGB(180, 180, 180));
HBRUSH TheChosenOne = (HBRUSH)HOLLOW_BRUSH;


// WINDOWS
HWND MainWnd = { };
HWND FieldWnd = { };
HWND GraphNameWnd = { };
HWND IsOrientedWnd = { };
HWND IsWeightedWnd = { };
HWND VerticeNameWnd = { };
HWND TransformPositionWnd = { };
HWND WeightWnd = { };


// OTHER
Field FieldInstance(NULL);
BOOL isLMBPressed = false;
BOOL isRMBPressed = false;
vector<Vertice> vertices = { };
UINT selectedVerticeID = { };




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }
	Field::FieldRegister();
	Vertice::VerticeRegister();

	MSG MainWndMessage = { 0 };


	//CreateWindow(L"MainWndClass", L"My Dumb Program", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1600, 900, NULL, NULL, NULL, NULL);
	MainWnd = CreateWindow(L"MainWndClass", L"My Dumb Program", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 100, 100, 1600, 900, NULL, NULL, NULL, NULL);
	while (GetMessage(&MainWndMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainWndMessage);
		DispatchMessage(&MainWndMessage);
	}
}






WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}





LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {


	switch (uMsg)
	{

	case WM_CREATE:
	{
		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	}
}



void MainWndAddMenus(HWND hWnd) {

}

void MainWndAddWidgets(HWND hWnd) {
	RECT r;
	HWND nhwnd;
	INT y = 11;

	GetClientRect(hWnd, &r);
	FieldWnd = CreateWindow(FIELD_WC, NULL, WS_CHILD | WS_VISIBLE, 10, 10, r.right - 250, r.bottom - 20, hWnd, (HMENU)FieldID, NULL, NULL);
	FieldInstance.SetWindow(FieldWnd);

	nhwnd = CreateWindowA("static", "Инспектор", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y, 218, 28, hWnd, NULL, NULL, NULL);
	SendMessageA(nhwnd, WM_SETFONT, (WPARAM)titlef, 0);

	GraphNameWnd = CreateWindowA("static", "NewGraph.graph", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)GraphName, NULL, NULL);
	SendMessageA(GraphNameWnd, WM_SETFONT, (WPARAM)titlef, 0);

	IsOrientedWnd = CreateWindowA("static", "Ориентированный: false", WS_CHILD | WS_VISIBLE, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)IsOriented, NULL, NULL);
	SendMessageA(IsOrientedWnd, WM_SETFONT, (WPARAM)textf, 0);

	IsWeightedWnd = CreateWindowA("static", "Взвешенный: false", WS_CHILD | WS_VISIBLE, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)IsWeighted, NULL, NULL);
	SendMessageA(IsWeightedWnd, WM_SETFONT, (WPARAM)textf, 0);

	nhwnd = CreateWindowA("static", "Вершина", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y += 28, 218, 28, hWnd, NULL, NULL, NULL);
	SendMessageA(nhwnd, WM_SETFONT, (WPARAM)titlef, 0);

	VerticeNameWnd = CreateWindowA("static", "Обозначение: ", WS_CHILD | WS_VISIBLE, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)VerticeName, NULL, NULL);
	SendMessageA(VerticeNameWnd, WM_SETFONT, (WPARAM)textf, 0);

	TransformPositionWnd = CreateWindowA("static", "Позиция: ", WS_CHILD | WS_VISIBLE, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)TransformPosition, NULL, NULL);
	SendMessageA(TransformPositionWnd, WM_SETFONT, (WPARAM)textf, 0);

	WeightWnd = CreateWindowA("static", "Вес: ", WS_CHILD | WS_VISIBLE | WS_DISABLED, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)Weight, NULL, NULL);
	SendMessageA(WeightWnd, WM_SETFONT, (WPARAM)textf, 0);

	y += 28;

	nhwnd = CreateWindowA("button", "УДАЛИТЬ ВЕРШИНУ", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)OnDeleteVerticeClicked, NULL, NULL);
	SendMessageA(nhwnd, WM_SETFONT, (WPARAM)CreateFontA(24, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "Comic Sans MS"), 0);
}

POINT GetLocalCoordinates(HWND hWnd) {
	RECT Rect;
	POINT pt;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	pt.x = Rect.left;
	pt.y = Rect.top;
	return pt;
}