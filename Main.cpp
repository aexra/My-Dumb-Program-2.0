#include "Main.h"
#include "Field.h"
#include "Vertex.h"
#include "Lib.h"
#include "ThemeManager.h"




/* GLOBAL VARIABLES */


// BRUSHES
HBRUSH fbkb;


// FONTS
HFONT hf = CreateFont(0, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Comic Sans MS");


// WINDOWS
HWND MainWnd = { };
HWND FieldWnd = { };
STATIC* GraphNameWnd = { };
FLAG* IsOrientedWnd = { };
FLAG* IsWeightedWnd = { };
HWND VertexNameStaticWnd = { };
HWND VertexNameEditWnd = { };
HWND VertexXEditWnd = { };
HWND VertexYEditWnd = { };
HWND TransformPositionWnd = { };
STATIC* WeightWnd = { };
BUTTON* DeleteButtonWnd = { };
HWND myAbsolutelyCoolestTipEver = { };


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
	fbkb = CreateSolidBrush(vRGB(tmr->GetPalette().fbk));;

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
			HPEN hPen;
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
			hPen = CreatePen(PS_SOLID, 1, vRGB(pal.fbd));
			HGDIOBJ oldp = SelectObject(hDC, hPen);
			Rectangle(hDC, r.right - 231, 10, r.right - 10, r.bottom - 10);

			EndPaint(hWnd, &ps);


			DeleteObject(SelectObject(hDC, old));
			DeleteObject(SelectObject(hDC, oldp));
			DeleteObject(hBrush);
			DeleteObject(hPen);

			break;
		}
		case WM_CTLCOLOREDIT:
		{
			HDC hdc = (HDC)wParam;
			COLORREF col = vRGB(tmr->GetPalette().fbk);
			SetTextColor(hdc, vRGB(tmr->GetPalette().text));
			SetBkColor(hdc, col);
			return (INT_PTR)fbkb;
		}
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			COLORREF col = vRGB(tmr->GetPalette().fbk);
			SetTextColor(hdc, vRGB(tmr->GetPalette().text));
			SetBkColor(hdc, col);
			return (INT_PTR)fbkb;
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
					if (!selectedVertexID) break;
					Vertex::DeleteSelected();
					FieldInstance.Redraw();
					break;
				}

				case OnLightThemeClicked:
				{
					tmr->SetLight();
					DeleteObject(linePen);
					linePen = CreatePen(PS_SOLID, 5, vRGB(tmr->GetPalette().vbd));
					InvalidateRect(MainWnd, NULL, FALSE);
					InvalidateRect(myAbsolutelyCoolestTipEver, NULL, FALSE);
					break;
				}

				case OnDarkThemeClicked:
				{
					tmr->SetDark();
					DeleteObject(linePen);
					linePen = CreatePen(PS_SOLID, 5, vRGB(tmr->GetPalette().vbd));
					InvalidateRect(MainWnd, NULL, FALSE);
					InvalidateRect(myAbsolutelyCoolestTipEver, NULL, FALSE);
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
	INT offset = 34;

	GetClientRect(hWnd, &r);
	FieldWnd = CreateWindow(FIELD_WC, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 10, 10, r.right - 250, r.bottom - 20, hWnd, (HMENU)FieldID, NULL, NULL);
	FieldInstance.SetWindow(FieldWnd);
	GetClientRect(FieldWnd, &fr);
	myAbsolutelyCoolestTipEver = CreateWindowA("static", "Используйте <ЛКМ> для создания и соединения вершин, а <ПКМ> для их перемещения", WS_CHILD | WS_VISIBLE | SS_CENTER,
		1, fr.bottom-20, fr.right-2, 20-2, FieldWnd, NULL, NULL, NULL);

	STATICPARAMS inspsp, titlesp, textsp;
	BUTTONPARAMS btp;

	inspsp.bdDefCol += 15;
	inspsp.bkCol = V3(9, 37, 46);

	titlesp.bkCol = tmr->GetPalette().fbk;
	titlesp.bdWidth = 2;

	textsp.bdWidth = 0;
	textsp.alignh = aligns::left;

	btp.bdDisCol = V3(56, 10, 10);
	btp.bdDefCol = V3(99, 16, 16);
	btp.bdHovCol = V3(124, 21, 21);
	btp.bdPreCol = V3(156, 25, 25);

	new STATIC(hWnd, "ИНСПЕКТОР", V3(r.right-230, y, 0), NULL, V3(219, offset, 0), inspsp);
	GraphNameWnd = new STATIC(hWnd, "NewGraph.graph", V3(r.right-230, y+=offset, 0), NULL, V3(219, offset+4, 24), titlesp);
	new STATIC(hWnd, "Ориентированный:", V3(r.right-230, y+=offset+4, 0), NULL, V3(185, offset, 24), textsp); 	/*st->Disable();*/
	IsOrientedWnd = new FLAG(hWnd, "txt", V3(r.right - 230 + 180 + 5, y, 0), V3(offset, offset, 3));
	new STATIC(hWnd, "Взвешенный:", V3(r.right-230, y+=offset, 0), NULL, V3(185, offset, 24), textsp);
	IsWeightedWnd = new FLAG(hWnd, "txt", V3(r.right - 230 + 180 + 5, y, 0), V3(offset, offset, 3));
	new STATIC(hWnd, "ВЕРШИНА", V3(r.right - 230, y += offset, 0), NULL, V3(219, offset, 0), titlesp);
	new STATIC(hWnd, "Имя:", V3(r.right - 230, y += offset, 0), NULL, V3(120, offset, 24), textsp);
	VertexNameEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_CENTER, r.right - 230 + 122, y, 97, offset, hWnd, (HMENU)VertexName, NULL, NULL);
	SendMessage(VertexNameEditWnd, WM_SETFONT, (WPARAM)hf, 0);
	new STATIC(hWnd, "Позиция:      X        Y", V3(r.right - 230, y += offset, 0), NULL, V3(219, offset, 24), textsp);
	VertexXEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, r.right - 229 + 94, y+=offset, 50, offset, hWnd, (HMENU)TransformPositionX, NULL, NULL);
	SendMessage(VertexXEditWnd, WM_SETFONT, (WPARAM)hf, 0);
	VertexYEditWnd = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | ES_NUMBER, r.right - 229 + 86 + 50 + 10, y, 50, offset, hWnd, (HMENU)TransformPositionY, NULL, NULL);
	SendMessage(VertexYEditWnd, WM_SETFONT, (WPARAM)hf, 0);
	WeightWnd = new STATIC(hWnd, "Вес: ", V3(r.right - 230, y += offset, 0), NULL, V3(218, offset, 0), textsp);
	DeleteButtonWnd = new BUTTON(hWnd, "УДАЛИТЬ ВЕРШИНУ", V3(r.right - 230, y += offset, 0), OnDeleteVertexClicked, V3(219, offset, 0), btp);
	DeleteButtonWnd->Disable();
}