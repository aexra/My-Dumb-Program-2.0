#include "Vertice.h"
#include "Field.h"
#include "Main.h"
#include "Vector2.h"
#include "ExtraOverloads.h"


extern vector<Vertice> vertices;
extern UINT selectedVerticeID;
extern BOOL isRMBPressed;
extern BOOL isLMBPressed;
extern Field FieldInstance;
extern enum selection_mode {mode1, mode2};
extern selection_mode selmode;
extern HWND GraphNameWnd;
extern HWND IsOrientedWnd;
extern HWND IsWeightedWnd;
extern HWND VerticeNameEditWnd;
extern HWND TransformPositionWnd;
extern HWND WeightWnd;
extern HWND FieldWnd;
extern CHAR BUFFER[40];
extern POINT OnFieldCursorPos;
extern HPEN linePen;

HPEN vPen = { };
HBRUSH vBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

Vertice* prelinkedVertice = nullptr;

int k = 0;

POINT lastHit = { };


Vertice::Vertice(UINT _id, HWND _hWnd, POINT _pt) {
	id = _id;
	hWnd = _hWnd;
	pt = _pt;
	weight = 0;
	name = to_string(Vertice::GetLastAvailableNumAsVerticeName());
	isSelected = false;
	isValid = true;
}

Vertice::Vertice() {
	id = NULL;
	weight = NULL;
	pt = { };
	center = { };
	hWnd = { };
	connections = { };
	name = "MissingName";
	isSelected = FALSE;
	isValid = FALSE;
}

Vertice::~Vertice() {
	DeleteObject(hWnd);
}

BOOL Vertice::IsValid() {
	return isValid;
}

UINT Vertice::GetID() {
	return id;
}
UINT Vertice::SetID(UINT _id) {
	id = _id;
	return id;
}
UINT Vertice::GenerateID() {
	vector<UINT> ids;
	for (Vertice v : vertices) {
		ids.push_back(v.GetID());
	}
	for (int i = minVerticeID; i < maxVerticeID; i++) {
		BOOL isFound = false;
		for (UINT id : ids) {
			if (id == i) {
				isFound = true;
				break;
			}
		}
		if (!isFound) {
			return i;
		}
	}
}
UINT Vertice::GetLastAvailableNumAsVerticeName()
{
	UINT lastFound = 1;
	BOOL changed = false;
	while (TRUE)
	{
		for (Vertice& v : vertices)
		{
			string vName = v.GetName();
			if (is_int(vName))
				if (stoi(vName) == lastFound)
				{
					lastFound++;
					changed = true;
				}
		}
		if (!changed) break;
		else changed = false;
	}
	return lastFound;
}
FLOAT	 Vertice::GetWeight() {
	return weight;
}
FLOAT	 Vertice::SetWeight(FLOAT _weight) {
	weight = _weight;
	return weight;
}

POINT Vertice::GetCenter()
{
	POINT _pt = { };
	_pt.x = pt.x + 50;
	_pt.y = pt.y + 50;
	return _pt;
}
POINT	 Vertice::GetPT() {
	return pt;
}
POINT	 Vertice::SetPT(POINT _pt) {
	POINT _cent;
	_cent.x = _pt.x + 50;
	_cent.y = _pt.y + 50;
	pt = _pt;
	return pt;
}


HWND&	 Vertice::GetWindow() {
	return hWnd;
}
HWND&	 Vertice::SetWindow(HWND& _hWnd) {
	hWnd = _hWnd;
	return hWnd;
}


vector<UINT> Vertice::GetConnections() {
	return connections;
}
vector<UINT> Vertice::SetConnections(vector<UINT> cons) {
	return connections;
}


UINT Vertice::Connect(UINT _id) {
	this->connections.push_back(_id);
	return _id;
}
UINT Vertice::Disconnect(UINT _id) {
	this->connections.erase(find(connections.begin(), connections.end(), _id));
	return _id;
}


string Vertice::GetName() {
	return name;
}
string Vertice::SetName(string _name) {
	name = _name;
	return name;
}

BOOL Vertice::IsSelected() {
	return isSelected;
}
BOOL Vertice::IsNear(const POINT _pt)
{
	return (sqrt(pow(this->pt.x + 50 - _pt.x, 2) + pow(this->pt.y + 50 - _pt.y, 2)) <= VERTICE_LINKING_RANGE);
}
void Vertice::Select() {
	isSelected = true;
	selectedVerticeID = id;
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
	UpdateInfoPanels();
}
void Vertice::Deselect() {
	isSelected = false;
	if (selectedVerticeID == id)
		selectedVerticeID = NULL;
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
	UpdateInfoPanels();
}
void Vertice::DeselectAll() {
	for (Vertice &v : vertices) 
	{
		v.Deselect();
	}
	UpdateInfoPanels();
}

int Vertice::GetVerticeIdx(UINT __id) {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].GetID() == __id) {
			return i;
		}
	}
	return NULL;
}
Vertice* Vertice::GetVertice(UINT __id) {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].GetID() == __id) {
			return &vertices[i];
		}
	}
	return nullptr;
}
Vertice* Vertice::GetVertice(HWND __hWnd) {
	return GetVertice(GetWindowLongA(__hWnd, GWL_ID));
}

void Vertice::DeleteSelected() {
	Vertice::DeleteVertice(selectedVerticeID);
}

void Vertice::DeleteVertice(UINT _id) {
	Vertice* v = Vertice::GetVertice(_id);
	int vec_idx = Vertice::GetVerticeIdx(_id);
	DestroyWindow(v -> GetWindow());
	vertices.erase(vertices.begin() + vec_idx);
	selectedVerticeID = NULL;
	UpdateInfoPanels();
}

RECT Vertice::GetRect()
{
	RECT r; 
	GetClientRect(hWnd, &r);
	return r;
}

void Vertice::DrawVertice(HDC _mDC)
{
	RECT r = GetRect();
	vPen = CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
	SelectObject(_mDC, vPen);

	Rectangle(_mDC, 0, 0, r.right, r.bottom);
	Ellipse(_mDC, 12, 12, 88, 88);

	vPen = CreatePen(PS_SOLID, 5, RGB(89, 89, 89));
	SelectObject(_mDC, GetStockObject(HOLLOW_BRUSH));
	SelectObject(_mDC, vPen);
	SetTextColor(_mDC, RGB(0, 0, 0));

	Ellipse(_mDC, 5, 5, 95, 95);
	DrawTextA(_mDC, (GetName()).c_str(), -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// Если эта вершина является выбранной
	if (IsSelected()) {
		vPen = CreatePen(PS_SOLID, 10, RGB(100, 149, 237));
		SelectObject(_mDC, vPen);
		SelectObject(_mDC, vBrush);
		Ellipse(_mDC, 12, 12, 88, 88);
	}
}

void Vertice::UpdateInfoPanels() {
	if (!selectedVerticeID) {
		
		GetWindowTextA(VerticeNameEditWnd, BUFFER, 30);
		if (string(BUFFER) != "")
		{
			SendMessageA(VerticeNameEditWnd, WM_SETTEXT, NULL, (LPARAM)string("").c_str());
			SetWindowLongPtrA(VerticeNameEditWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_DISABLED);
		}
		
		GetWindowTextA(TransformPositionWnd, BUFFER, 30);
		if (string(BUFFER) != "Позиция: ")
			SendMessageA(TransformPositionWnd, WM_SETTEXT, NULL, (LPARAM)string("Позиция: ").c_str());
		
		GetWindowTextA(WeightWnd, BUFFER, 30);
		if (string(BUFFER) != "Вес: ")
			SendMessageA(WeightWnd, WM_SETTEXT, NULL, (LPARAM)string("Вес: ").c_str());
		return;
	}

	Vertice &v = *Vertice::GetVertice(selectedVerticeID);

	GetWindowTextA(VerticeNameEditWnd, BUFFER, 30);
	if (string(BUFFER) != v.GetName())
	{
		SendMessageA(VerticeNameEditWnd, WM_SETTEXT, NULL, (LPARAM)v.GetName().c_str());
		SetWindowLongPtrA(VerticeNameEditWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
	}

	GetWindowTextA(TransformPositionWnd, BUFFER, 30);
	if (string(BUFFER) != "Позиция: (" + to_string(v.GetPT().x) + "; " + to_string(v.GetPT().y) + ")")
		SendMessageA(TransformPositionWnd, WM_SETTEXT, NULL, (LPARAM)string("Позиция: (" + to_string(v.GetPT().x) + "; " + to_string(v.GetPT().y) + ")").c_str());

	GetWindowTextA(WeightWnd, BUFFER, 30);
	if (string(BUFFER) != "Вес: " + to_string(v.GetWeight()))
		SendMessageA(WeightWnd, WM_SETTEXT, NULL, (LPARAM)string("Вес: " + to_string(v.GetWeight())).c_str());
}

Vertice* Vertice::GetSelected()
{
	return Vertice::GetVertice(selectedVerticeID);
}


void Vertice::VerticeRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Vertice::VerticeWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = VERTICE_WC;
	RegisterClass(&wc);
}
void Vertice::VerticeUnregister(void)
{
	UnregisterClass(VERTICE_WC, NULL);
}
//inline void Vertice::OnLeftMouseMove()
//{
//	
//}
//inline void Vertice::OnRightMouseMove()
//{
//
//}


LRESULT CALLBACK Vertice::VerticeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Vertice* v = Vertice::GetVertice(hWnd);

	switch (uMsg) {

		case WM_PAINT:
		{
			PAINTSTRUCT		vps;
			HDC							VDC;
			HDC							memDC;
			HBITMAP					memBM;
			RECT						r;

			// щя будет двойная буферизация

			GetClientRect(hWnd, &r);

			VDC = BeginPaint(hWnd, &vps);

			memDC = CreateCompatibleDC(VDC);
			memBM = CreateCompatibleBitmap(VDC, 100, 100);
			SelectObject(memDC, memBM);

			v -> DrawVertice(memDC);

			BitBlt(VDC, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);

			EndPaint(hWnd, &vps);

			DeleteObject(memBM);
			DeleteDC(memDC);

			//Sleep(1000 / 6000);
			return 0;
		}

		/*case WM_ERASEBKGND:
		{
			return 0;
		}*/

		case WM_MOVE:
		{
			// это работает на ноуте
			//RECT rect;
			//GetClientRect(hWnd, &rect);
			//RedrawWindow(hWnd, &rect, NULL, RDW_ERASE);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			isLMBPressed = true;
			SetCapture(hWnd);
			if (v -> IsSelected()) {
					v -> Deselect();
			}
			else {
				if (selmode == mode1)
					Vertice::DeselectAll();
				v -> Select();
			}

			break;
		}

		case WM_LBUTTONUP:
		{
			isLMBPressed = false;
			InvalidateRect(FieldWnd, NULL, FALSE);
			UpdateWindow(FieldWnd);
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			if (prelinkedVertice != nullptr)
			{
				// ВРЕМЕННОЕ РЕШЕНИЕ - TODO: СОЕДИНЕНИЕ ВЕРШИН
				InvalidateRect(prelinkedVertice -> GetWindow(), NULL, FALSE);
				UpdateWindow(prelinkedVertice -> GetWindow());
				/*if ((find(v.GetConnections().begin(), v.GetConnections().end(), prelinkedVertice -> GetID()) == v.GetConnections().end())) v.Connect(prelinkedVertice->GetID());
				OutputDebugStringA(to_string(v.GetConnections().size()).c_str());*/
				prelinkedVertice = nullptr;
			}
			ReleaseCapture();	
			break;
		}

		case WM_RBUTTONDOWN:
		{
			isRMBPressed = true;
			SetCapture(hWnd);
			break;
		}

		case WM_RBUTTONUP:
		{
			isRMBPressed = false;
			ReleaseCapture();
			break;
		}

		case WM_MOUSEMOVE:
		{
			//
			//		Правая мышб для перемещения
			//
			if (isRMBPressed)
			{
				RECT rc;
				POINT ptCursor, vp, dest;
				RECT parentRect;
				LONG width, length;

				GetWindowRect(hWnd, &rc);
				GetClientRect(FieldWnd, &parentRect);
				vp = GetLocalCoordinates(hWnd);

				ptCursor.x = GET_X_LPARAM(lParam);
				ptCursor.y = GET_Y_LPARAM(lParam);

				length = rc.right - rc.left;
				width = rc.bottom - rc.top;

				dest.x = vp.x + ptCursor.x - (length) / 2;
				dest.y = vp.y + ptCursor.y - (width) / 2;

				if (dest.x <= 0 || dest.y <= 0 || dest.x + width >= parentRect.right || dest.y + length >= parentRect.bottom) {
					return DefWindowProc(hWnd, uMsg, wParam, lParam);
				}
				for (Vertice& v2 : vertices) {
					if (v -> GetID() == v2.GetID()) continue;
					POINT vpt = v2.GetPT();
					if (sqrt(pow(abs(vpt.x - dest.x), 2) + pow(abs(vpt.y - dest.y), 2)) > 100 + VERTICE_DISTANCE_ERROR) continue;
					else return DefWindowProc(hWnd, uMsg, wParam, lParam);;
				}

				v -> SetPT(dest);

				InvalidateRect(FieldWnd, GetLocalRect(hWnd), FALSE);	//		Это работает на пк
				UpdateWindow(FieldWnd);													//

				MoveWindow(hWnd, v -> GetPT().x, v -> GetPT().y, 100, 100, TRUE);

			}
			//
			//		Левая мышб для выделения и соединения
			//
			if (isLMBPressed)
			{
				// Если вершина не выделена, выделяем (решение бага с лишним deselect-ом)
				if (!(v -> IsSelected()))
					v -> Select();
				POINT cursor = { };
				POINT vloc = v -> GetPT();	// координаты вершины на поле
				POINT startpos = { 50, 50 };

				// Получим координаты положения курсора на ВЕРШИНЕ
				cursor.x = GET_X_LPARAM(lParam);
				cursor.y = GET_Y_LPARAM(lParam);

				// Если последнее положение соответствует текущему, нам не нужно его перерисовывать.
				if (cursor == lastHit) break;
				else lastHit = cursor;

				// Получим контексты девайсов
				HDC FDC = GetDC(FieldWnd);
				HDC VDC = GetDC(hWnd);

				// Подготовим всё необходимое для рисования линии на ПОЛЕ
				// Испульзуем двойную буферизацию для исключения мерцания
				RECT fr = FieldInstance.GetRect();
				HDC memFDC = CreateCompatibleDC(FDC);
				HBITMAP memFBM = CreateCompatibleBitmap(FDC, fr.right, fr.bottom);
				SelectObject(memFDC, memFBM);

				// Отрисуем пустое поле
				FieldInstance.DrawField(memFDC);

				// Подготовим всё необходимое для рисования линии на ВЕРШИНЕ
				// Испульзуем двойную буферизацию для исключения мерцания
				RECT vr = v -> GetRect();
				HDC memVDC = CreateCompatibleDC(VDC);
				HBITMAP memVBM = CreateCompatibleBitmap(VDC, vr.right, vr.bottom);
				SelectObject(memVDC, memVBM);

				// Отрисуем вершину
				v -> DrawVertice(memVDC);

				// Выберем перо для рисования линии
				SelectObject(memFDC, linePen);
				SelectObject(memVDC, linePen);

				// Проверим возможность соединиться
				for (Vertice& v1 : vertices)
				{
					if (v1 == *v) continue;
					/*OutputDebugStringA((to_string(cursor.x + vloc.x) + " - " + to_string(cursor.y + vloc.y) + "\t\t").c_str());
					OutputDebugStringA((to_string(v1.GetPT().x + 50) + " - " + to_string(v1.GetPT().y + 50) + "\t\t").c_str());
					OutputDebugStringA((to_string(v1.IsNear(cursor + vloc)) + "\n").c_str());*/
					if (v1.IsNear(cursor + vloc))
					{

						// Фикс бага отображения линии при быстрой смене фокуса
						if (prelinkedVertice != nullptr && prelinkedVertice != &v1) 
						{
							InvalidateRect(prelinkedVertice->GetWindow(), NULL, FALSE);
							UpdateWindow(prelinkedVertice->GetWindow());
						}

						// Запоминаем вершину на которой висит фокус (пригодится)
						prelinkedVertice = &v1;

						// Получим координаты центра другой вершины
						POINT pt1 = v1.GetCenter();

						// Подготовим всё необходимое для рисования линии на ВЕРШИНЕ1
						// Испульзуем двойную буферизацию для исключения мерцания
						RECT vr1 = v1.GetRect();
						HDC VDC1 = GetDC(v1.GetWindow());
						HDC memVDC1 = CreateCompatibleDC(VDC1);
						HBITMAP memVBM1 = CreateCompatibleBitmap(VDC1, vr1.right, vr1.bottom);
						SelectObject(memVDC1, memVBM1);

						// Отрисуем другую вершину
						v1.DrawVertice(memVDC1);

						// Линия под вершинами между ними
						DrawLine(memFDC, vloc.x + 50, vloc.y + 50, pt1.x, pt1.y);										
						
						// Линия на первой вершине
						POINT startv = intersectionPoints(POINT{ 50, 50 }, pt1 - vloc, POINT{ 50, 50 }, 48)[0];
						DrawLine(memVDC, startv.x, startv.y, pt1.x - vloc.x, pt1.y - vloc.y);										
						
						// Линия на второй вершине
						POINT startv1 = intersectionPoints(POINT{ 50, 50 }, vloc - pt1 + 100, POINT{ 50, 50 }, 48)[0];
						DrawLine(memVDC1, startv1.x, startv1.y, vloc.x - pt1.x + 50 + 50, vloc.y - pt1.y + 50 + 50);		
						
						// Перенесем изображения
						BitBlt(FDC, 0, 0, fr.right, fr.bottom, memFDC, 0, 0, SRCCOPY);
						BitBlt(VDC, 0, 0, vr.right, vr.bottom, memVDC, 0, 0, SRCCOPY);
						BitBlt(VDC1, 0, 0, vr.right, vr.bottom, memVDC1, 0, 0, SRCCOPY);

						// Уничтожим использованные объекты
						ReleaseDC(FieldWnd, FDC);
						ReleaseDC(hWnd, VDC);
						ReleaseDC(v1.GetWindow(), VDC1);
						DeleteDC(memFDC);
						DeleteDC(memVDC);
						DeleteDC(memVDC1);
						DeleteBitmap(memFBM);
						DeleteBitmap(memVBM);
						DeleteBitmap(memVBM1);

						return DefWindowProc(hWnd, uMsg, wParam, lParam);
					}
					else 
					{
						InvalidateRect(v1.GetWindow(), NULL, FALSE);
						UpdateWindow(v1.GetWindow());
					}
				}

				// Отрисуем линию
				DrawLine(memFDC, vloc.x + 50, vloc.y + 50, cursor.x + vloc.x, cursor.y + vloc.y);
				DrawLine(memVDC, 50, 50, cursor.x, cursor.y);

				// Перенесем изображения
				BitBlt(FDC, 0, 0, fr.right, fr.bottom, memFDC, 0, 0, SRCCOPY);
				BitBlt(VDC, 0, 0, vr.right, vr.bottom, memVDC, 0, 0, SRCCOPY);

				// Уничтожим использованные объекты
				ReleaseDC(FieldWnd, FDC);
				ReleaseDC(hWnd, VDC);
				DeleteDC(memFDC);
				DeleteDC(memVDC);
				DeleteBitmap(memFBM);
				DeleteBitmap(memVBM);
			}


			break;
		}

		case WM_CREATE:
		{
			break;
		}

		case WM_DESTROY:
		{
			DestroyWindow(hWnd);
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

BOOL operator == (Vertice& _Left, Vertice& _Right)
{
	return (_Left.GetID() == _Right.GetID());
}