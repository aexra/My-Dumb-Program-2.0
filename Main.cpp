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
HWND DeleteButtonWnd = { };


// ENUMERATORS
enum selection_mode { mode1, mode2 };


// THREADS HANDLES
HANDLE hThread2;


// OTHER
Field FieldInstance(NULL);
BOOL isLMBPressed = false;
BOOL isRMBPressed = false;
vector<Vertice> vertices = { };
UINT selectedVerticeID = { };
selection_mode selmode = mode1;
CHAR BUFFER[40];
HPEN linePen = { };


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }
	Field::FieldRegister();
	Vertice::VerticeRegister();

	MSG MainWndMessage = { 0 };

	linePen = CreatePen(PS_SOLID, 5, RGB(89, 89, 89));

	//hThread2 = CreateThread(NULL, 0, LineDrawerThreadProc, (LPVOID)THREAD2, 0, NULL);


	//CreateWindow(L"MainWndClass", L"My Dumb Program", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1600, 900, NULL, NULL, NULL, NULL);
	MainWnd = CreateWindow(L"MainWndClass", L"My Dumb Program 2.0", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 100, 100, 1600, 900, NULL, NULL, NULL, NULL);
	while (GetMessage(&MainWndMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainWndMessage);
		DispatchMessage(&MainWndMessage);
	}
}


//DWORD WINAPI LineDrawerThreadProc(LPVOID lParam)
//{
//	while (true)
//	{
//		if (selectedVerticeID)
//		{
//			OutputDebugStringA("");
//
//			Vertice& v = *Vertice::GetVertice(selectedVerticeID);
//			POINT vloc = v.GetPT();
//			HDC FDC = GetDC(FieldWnd);
//
//			//OutputDebugStringA((to_string(vloc.x) + ", " + to_string(vloc.y) + " -> " + to_string(OnFieldCursorPos.x) + ", " + to_string(OnFieldCursorPos.y) + "\n").c_str());
//
//			SelectObject(FDC, linePen);
//			SelectObject(*v.GetHDC(), linePen);
//			DrawLine(FDC, vloc.x+50, vloc.y+50, OnFieldCursorPos.x, OnFieldCursorPos.y+500);
//			DrawLine(*v.GetHDC(), 0, 0, OnFieldCursorPos.x, OnFieldCursorPos.y);
//
//			ReleaseDC(FieldWnd, FDC);
//		}
//	}
//	return 0;
//}



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


void Missing(HWND hWnd) {
	MessageBoxA(hWnd, "А эту фичу разраб еще не сделал.", "А где?", MB_OK);
}


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {


	switch (uMsg)
	{
		case WM_PAINT:
		{
			HBRUSH hBrush;
			PAINTSTRUCT ps;
			HDC hDC;
			RECT r;

			hDC = BeginPaint(hWnd, &ps);
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			GetClientRect(hWnd, &r);

			Rectangle(hDC, r.right - 231, 10, r.right - 10, r.bottom - 10);

			EndPaint(hWnd, &ps);

			DeleteObject(hDC);
			DeleteObject(hBrush);

			break;
		}

		case WM_COMMAND:
		{
			switch (wParam)
			{

				case OnInfoClicked:
				{
					MessageBoxA(hWnd, "Донской Государственный Технический Университет\nФакультет: ИиВТ\nКафедра: ПОВТиАС (09.03.04)\nГруппа: ВПР12\nСтудент: Фомин Н. А.\n\nПредполагается, что это будет простенький редактор графов.", "Справка", MB_OK);
					break;
				}

				case OnNewClicked:
				{
					Missing(hWnd);
					break;
				}

				case OnLoadGraphClicked:
				{
					Missing(hWnd);
					break;
				}

				case OnSaveClicked:
				{
					Missing(hWnd);
					break;
				}

				case OnSaveAsClicked:
				{
					Missing(hWnd);
					break;
				}

				case OnExitClicked:
				{
					PostQuitMessage(0);
					break;
				}

				case OnGitSourceClicked:
				{
					ShellExecute(0, 0, L"http://github.com/EvilPrincess/My-Dumb-Program-2.0.git", 0, 0, SW_SHOW);
					break;
				}

				case OnDeleteVerticeClicked:
				{
					if (selectedVerticeID) Vertice::DeleteSelected();
					SendMessageA(DeleteButtonWnd, WM_KILLFOCUS, NULL, NULL);
					break;
				}

			}

			break;
		}

		case WM_TIMER:
		{
			switch (wParam)
			{
				case INFOPANEL_REFRESH_IDT:
				{
					Vertice::UpdateInfoPanels();
					break;
				}
			}
			break;
		}

		case WM_CREATE:
		{
			MainWndAddMenus(hWnd);
			MainWndAddWidgets(hWnd);
			SetTimer(hWnd, INFOPANEL_REFRESH_IDT, INFOPANEL_REFRESH_RATE, (TIMERPROC)NULL);
			break;
		}

		case WM_DESTROY:
		{
			selectedVerticeID = NULL;
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
	// Менюшка вкладок
	HMENU RootMenu = CreateMenu();

	// Подменюшка Файл'а
	HMENU FileMenu = CreateMenu();

	// Подменюшка ЧаВО
	HMENU WhatMenu = CreateMenu();

	// Подменюшка Темы
	HMENU ThemeMenu = CreateMenu();

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)FileMenu, L"Файл");

	AppendMenu(FileMenu, MF_STRING, OnNewClicked, L"Новый граф (CTRL+N)");
	AppendMenu(FileMenu, MF_STRING, OnLoadGraphClicked, L"Загрузить граф (CTRL+L)");
	AppendMenu(FileMenu, MF_STRING, OnSaveClicked, L"Сохранить (CTRL+S)");
	AppendMenu(FileMenu, MF_STRING, OnSaveAsClicked, L"Сохранить как (CTRL+ALT+S)");
	AppendMenu(FileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(FileMenu, MF_STRING, OnExitClicked, L"Выйти");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)WhatMenu, L"ЧаВо");

	AppendMenu(WhatMenu, MF_STRING, OnInfoClicked, L"Справка");
	AppendMenu(WhatMenu, MF_STRING, OnGitSourceClicked, L"Git Source");


	SetMenu(hWnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {
	RECT r;
	HWND nhwnd;
	INT y = 11;

	GetClientRect(hWnd, &r);
	FieldWnd = CreateWindow(FIELD_WC, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 10, 10, r.right - 250, r.bottom - 20, hWnd, (HMENU)FieldID, NULL, NULL);
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

	DeleteButtonWnd = CreateWindowA("button", "УДАЛИТЬ ВЕРШИНУ", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)OnDeleteVerticeClicked, NULL, NULL);
	SendMessageA(nhwnd, WM_SETFONT, (WPARAM)CreateFontA(24, 8, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "Comic Sans MS"), 0);
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

RECT* GetLocalRect(HWND hWnd)
{
	RECT Rect;
	GetWindowRect(hWnd, &Rect);
	MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&Rect, 2);
	return &Rect;
}