#include "Main.h"
#include "Field.h"
#include "Vertex.h"
#include "Lib.h"
#include "ThemeManager.h"




/* GLOBAL VARIABLES */


// BRUSHES
HBRUSH SysGrey = (HBRUSH)COLOR_WINDOW;
HBRUSH TitleBrush = CreateSolidBrush(RGB(180, 180, 180));
HBRUSH TheChosenOne = (HBRUSH)HOLLOW_BRUSH;


// WINDOWS
HWND MainWnd = { };
HWND FieldWnd = { };
STATIC* GraphNameWnd = { };
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


// THEME MANAGER SINGLETON
ThemeManager* tmr;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	InitLib();
	tmr = ThemeManager::GetInstance();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), L"MainWndClass", MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }
	Field::FieldRegister();
	Vertex::VertexRegister();

	MSG MainWndMessage = { 0 };

	linePen = CreatePen(PS_SOLID, 5, vRGB(tmr->GetPalette().vbd));

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
			PALETTE pal = tmr->GetPalette();
			HBRUSH hBrush;
			PAINTSTRUCT ps;
			HDC hDC;
			RECT r;

			hDC = BeginPaint(hWnd, &ps);

			hBrush = CreateSolidBrush(vRGB(pal.bk));
			HGDIOBJ old = SelectObject(hDC, hBrush);
			GetClientRect(hWnd, &r);

			// Фон окна
			Rectangle(hDC, r.left, r.top, r.right, r.bottom);

			hBrush = CreateSolidBrush(vRGB(pal.fbk));
			DeleteObject(SelectObject(hDC, hBrush));

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
							
							v->GetPT().x = min(stoi(sx), FieldInstance.GetRect().right - FieldInstance.GetRect().left - 101);
							MoveWindow(v->GetWindow(), v->GetPT().x, v->GetPT().y, 100, 100, TRUE);
							FieldInstance.Redraw();
							break;
						}
						case TransformPositionY:
						{
							Vertex* v = Vertex::GetSelected();
							if (v == nullptr) break;
							GetWindowTextA(VertexYEditWnd, BUFFER, 5);
							string sy = BUFFER;
							if (sy == "" || sy == "0") sy = "1";

							v->GetPT().y = min(stoi(sy), FieldInstance.GetRect().bottom - FieldInstance.GetRect().top - 101);
							MoveWindow(v->GetWindow(), v->GetPT().x, v->GetPT().y, 100, 100, TRUE);
							FieldInstance.Redraw();
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

				case OnLightThemeClicked:
				{
					tmr->SetLight();
					DeleteObject(linePen);
					linePen = CreatePen(PS_SOLID, 5, vRGB(tmr->GetPalette().vbd));
					break;
				}

				case OnDarkThemeClicked:
				{
					tmr->SetDark();
					DeleteObject(linePen);
					linePen = CreatePen(PS_SOLID, 5, vRGB(tmr->GetPalette().vbd));
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

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)ThemeMenu, L"Изменить тему");

	AppendMenu(ThemeMenu, MF_STRING, OnLightThemeClicked, L"Светлая тема");
	AppendMenu(ThemeMenu, MF_STRING, OnDarkThemeClicked, L"Темная тема");

	SetMenu(hWnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {
	RECT r;
	RECT fr;
	HWND nhwnd;
	INT y = 11;

	GetClientRect(hWnd, &r);
	FieldWnd = CreateWindow(FIELD_WC, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 10, 10, r.right - 250, r.bottom - 20, hWnd, (HMENU)FieldID, NULL, NULL);
	FieldInstance.SetWindow(FieldWnd);
	GetClientRect(FieldWnd, &fr);
	CreateWindowA("static", "Используйте <ЛКМ> для создания и соединения вершин, а <ПКМ> для их перемещения", WS_CHILD | WS_VISIBLE | SS_CENTER,
		1, fr.bottom-20, fr.right-2, 20-2, FieldWnd, NULL, NULL, NULL);

	new STATIC(hWnd, "ИНСПЕКТОР", V3(r.right-230, y, 0), NULL, V3(219, 28, 0));
	GraphNameWnd = new STATIC(hWnd, "NewGraph.graph", V3(r.right-230, y+=28, 0), NULL, V3(219, 28, 0));
	new STATIC(hWnd, "Ориентация:", V3(r.right-230, y+=28, 0), NULL, V3(190, 28, 0)); 	/*st->Disable();*/
	new STATIC(hWnd, "Взвешенность:", V3(r.right-230, y+=28, 0), NULL, V3(190, 28, 0));
	new STATIC(hWnd, "ВЕРШИНА", V3(r.right - 230, y += 28, 0), NULL, V3(219, 28, 0));
	new STATIC(hWnd, "Обозначение:", V3(r.right - 230, y += 28, 0), NULL, V3(120, 28, 0));
	VertexNameEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | WS_DISABLED, r.right - 230 + 122, y, 97, 28, hWnd, (HMENU)VertexName, NULL, NULL);
	new STATIC(hWnd, "Позиция:      X      Y", V3(r.right - 230, y += 28, 0), NULL, V3(219, 28, 0));
	y += 28;
	VertexXEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, r.right - 229 + 86, y, 50, 28, hWnd, (HMENU)TransformPositionX, NULL, NULL);
	VertexYEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, r.right - 229 + 86 + 50 + 10, y, 50, 28, hWnd, (HMENU)TransformPositionY, NULL, NULL);
	WeightWnd = CreateWindowA("static", "Вес: ", WS_CHILD | WS_VISIBLE | WS_DISABLED, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)Weight, NULL, NULL);
	y += 28;
	DeleteButtonWnd = CreateWindowA("button", "УДАЛИТЬ ВЕРШИНУ", WS_CHILD | WS_VISIBLE | SS_CENTER, r.right - 229, y += 28, 218, 28, hWnd, (HMENU)OnDeleteVertexClicked, NULL, NULL);
}