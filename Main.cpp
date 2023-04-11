#include "Main.h"
#include "Field.h"
#include "Vertex.h"





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
HWND VertexNameStaticWnd = { };
HWND VertexNameEditWnd = { };
HWND VertexXEditWnd = { };
HWND VertexYEditWnd = { };
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
BOOL isOrientedGraph = false;
BOOL isEditingX = false;
BOOL isEditingY = false;
vector<Vertex*> vertices = { };
UINT selectedVertexID = { };
selection_mode selmode = mode1;
CHAR BUFFER[40];
HPEN linePen = { };


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }
	Field::FieldRegister();
	Vertex::VertexRegister();

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
			INT bkcol = 240;

			hDC = BeginPaint(hWnd, &ps);

			hBrush = CreateSolidBrush(RGB(bkcol, bkcol, bkcol));
			HGDIOBJ old = SelectObject(hDC, hBrush);
			GetClientRect(hWnd, &r);

			// инфопанель
			Rectangle(hDC, r.right - 231, 10, r.right - 10, r.bottom - 10);

			EndPaint(hWnd, &ps);

			DeleteObject(SelectObject(hDC, old));
			DeleteObject(hBrush);

			break;
		}

		case WM_COMMAND:
		{
			switch (HIWORD(wParam))
			{
				case EN_CHANGE:
				{
					switch(LOWORD(wParam))
					{
						case VertexName:
						{
							Vertex* v = Vertex::GetSelected();
							if (v == nullptr) break;
							GetWindowTextA(VertexNameEditWnd, BUFFER, 8);
							v->SetName(BUFFER);
							InvalidateRect(v->GetWindow(), NULL, FALSE);
							UpdateWindow(v->GetWindow());
							break;
						}
						case TransformPositionX:
						{
							Vertex* v = Vertex::GetSelected();
							if (v == nullptr) break;
							GetWindowTextA(VertexXEditWnd, BUFFER, 5);
							string sx = BUFFER;
							if (sx == "" || sx == "1") sx = "1";
							
							v->GetPT().x = stoi(sx);
							MoveWindow(v->GetWindow(), v->GetPT().x, v->GetPT().y, 100, 100, TRUE);
							break;
						}
						case TransformPositionY:
						{
							Vertex* v = Vertex::GetSelected();
							if (v == nullptr) break;
							GetWindowTextA(VertexYEditWnd, BUFFER, 5);
							string sy = BUFFER;
							if (sy == "" || sy == "0") sy = "1";

							v->GetPT().y = stoi(sy);
							MoveWindow(v->GetWindow(), v->GetPT().x, v->GetPT().y, 100, 100, TRUE);
							break;
						}
					}
					break;
				}
				case WM_SETFOCUS:
				{
					switch(LOWORD(wParam))
					{
						case TransformPositionX:
						{
							isEditingX = true;
							break;
						}
						case TransformPositionY:
						{
							isEditingY = true;
							break;
						}
					}
					break;
				}
				case EN_KILLFOCUS:
				{
					switch (LOWORD(wParam))
					{
						case TransformPositionX:
						{
							isEditingX = false;
							break;
						}
						case TransformPositionY:
						{
							isEditingY = false;
							break;
						}
					}
					break;
				}
			}

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

				case OnClearAllClicked:
				{
					for (int i = vertices.size() - 1; i > -1; i--)
					{
						Vertex::DeleteVertex(vertices[i] -> GetID());
					}
					FieldInstance.Redraw();
					break;
				}

				case OnDeleteVertexClicked:
				{
					if (selectedVertexID) Vertex::DeleteSelected();
					SendMessageA(DeleteButtonWnd, WM_KILLFOCUS, NULL, NULL);
					FieldInstance.Redraw();
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
					Vertex::UpdateInfoPanels();
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
			selectedVertexID = NULL;
			_CrtDumpMemoryLeaks();
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
	AppendMenu(FileMenu, MF_STRING, OnClearAllClicked, L"Очистить всё");
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
	CreateWindowA("static", "Используйте <ЛКМ> для создания и соединения вершин, а <ПКМ> для их перемещения", WS_CHILD | WS_VISIBLE | SS_CENTER,
		10+1, r.bottom-20-20+9, r.right-250-2, 20, hWnd, NULL, NULL, NULL);

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

	VertexNameStaticWnd = CreateWindowA("static", "Обозначение: ", WS_CHILD | WS_VISIBLE, r.right - 229, y += 28, 109, 28, hWnd, NULL, NULL, NULL);
	VertexNameEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | WS_DISABLED, r.right - 229 + 109, y, 109, 28, hWnd, (HMENU)VertexName, NULL, NULL);
	SendMessageA(VertexNameStaticWnd, WM_SETFONT, (WPARAM)textf, 0);
	SendMessageA(VertexNameEditWnd, WM_SETFONT, (WPARAM)textf, 0);

	TransformPositionWnd = CreateWindowA("static", "Позиция:          x             y", WS_CHILD | WS_VISIBLE, r.right - 229, y += 28, 218, 28, hWnd, NULL, NULL, NULL);
	SendMessageA(TransformPositionWnd, WM_SETFONT, (WPARAM)textf, 0);

	y += 28;
	VertexXEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, r.right - 229 + 86, y, 50, 28, hWnd, (HMENU)TransformPositionX, NULL, NULL);
	VertexYEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, r.right - 229 + 86 + 50 + 10, y, 50, 28, hWnd, (HMENU)TransformPositionY, NULL, NULL);
	SendMessageA(VertexXEditWnd, WM_SETFONT, (WPARAM)textf, 0);
	SendMessageA(VertexYEditWnd, WM_SETFONT, (WPARAM)textf, 0);

	WeightWnd = CreateWindowA("static", "Вес: ", WS_CHILD | WS_VISIBLE | WS_DISABLED, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)Weight, NULL, NULL);
	SendMessageA(WeightWnd, WM_SETFONT, (WPARAM)textf, 0);

	y += 28;

	DeleteButtonWnd = CreateWindowA("button", "УДАЛИТЬ ВЕРШИНУ", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)OnDeleteVertexClicked, NULL, NULL);
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