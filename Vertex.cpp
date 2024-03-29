﻿#include "Vertex.h"
#include "Field.h"
#include "Main.h"
#include "Lib.h"
#include "Vector2.h"
#include "ExtraOverloads.h"
#include "ThemeManager.h"
#include "Edge.h"

extern vector<Vertex*> vertices;
extern UINT selectedVertexID;
extern BOOL isRMBPressed;
extern BOOL isLMBPressed;
extern BOOL isOrientedGraph;
extern Field FieldInstance;
extern enum selection_mode {mode1, mode2};
extern selection_mode selmode;
extern HWND MainWnd;
extern HWND GraphNameWnd;
extern HWND IsOrientedWnd;
extern HWND IsWeightedWnd;
extern HWND VertexNameEditWnd;
extern HWND TransformPositionWnd;
extern HWND VertexXEditWnd;
extern HWND VertexYEditWnd;
extern STATIC* WeightWnd;
extern HWND FieldWnd;
extern CHAR BUFFER[40];
extern POINT OnFieldCursorPos;
extern HPEN linePen;
extern HPEN fPen;
extern BOOL isEditingX;
extern BOOL isEditingY;
extern ThemeManager* tmr;
extern BUTTON* DeleteButtonWnd;

HPEN vPen = { };
HBRUSH vBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

Vertex* prelinkedVertex = nullptr;
int k = 0;

RECT cursorline = { };
POINT lastHit = { };


Vertex::Vertex(UINT _id, HWND _hWnd, POINT _pt) {
	id = _id;
	hWnd = _hWnd;
	pt = _pt;
	weight = 0;
	name = to_string(Vertex::GetLastAvailableNumAsVertexName());
	isSelected = false;
	isValid = true;
}

Vertex::Vertex() {
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

Vertex::~Vertex() {
	DeleteObject(hWnd);
}

BOOL Vertex::IsValid() {
	return isValid;
}

UINT Vertex::GetID() {
	return id;
}
UINT Vertex::SetID(UINT _id) {
	id = _id;
	return id;
}
UINT Vertex::GenerateID() {
	vector<UINT> ids;
	for (Vertex* v : vertices) {
		ids.push_back(v -> GetID());
	}
	for (int i = minVertexID; i < maxVertexID; i++) {
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
UINT Vertex::GetLastAvailableNumAsVertexName()
{
	UINT lastFound = 1;
	BOOL changed = false;
	while (TRUE)
	{
		for (Vertex* v : vertices)
		{
			string vName = v -> GetName();
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
FLOAT	 Vertex::GetWeight() {
	return weight;
}
FLOAT	 Vertex::SetWeight(FLOAT _weight) {
	weight = _weight;
	return weight;
}

POINT Vertex::GetCenter()
{
	POINT _pt = { };
	_pt.x = pt.x + 50;
	_pt.y = pt.y + 50;
	return _pt;
}
POINT&	 Vertex::GetPT() {
	return pt;
}
POINT	 Vertex::SetPT(POINT _pt) {
	POINT _cent;
	_cent.x = _pt.x + 50;
	_cent.y = _pt.y + 50;
	pt = _pt;
	return pt;
}


HWND&	 Vertex::GetWindow() {
	return hWnd;
}
HWND&	 Vertex::SetWindow(HWND& _hWnd) {
	hWnd = _hWnd;
	return hWnd;
}


vector<UINT> Vertex::GetConnections() {
	return connections;
}
vector<UINT> Vertex::SetConnections(vector<UINT> cons) {
	return connections;
}


UINT Vertex::Connect(UINT _id) {
	if (find(this->connections.begin(), this->connections.end(), _id) == this->connections.end())
	{
		this->connections.push_back(_id);
		FieldInstance.Redraw();
		return _id;
	}
	else
		return 0;
}
UINT Vertex::Disconnect(UINT _id) {
	this->connections.erase(find(connections.begin(), connections.end(), _id));
	return _id;
}


string Vertex::GetName() {
	return name;
}
string Vertex::SetName(string _name) {
	string prevname = name;
	name = _name;
	return prevname;
}

BOOL Vertex::IsSelected() {
	return isSelected;
}
BOOL Vertex::IsNear(const POINT _pt)
{
	return (sqrt(pow(this->pt.x + 50 - _pt.x, 2) + pow(this->pt.y + 50 - _pt.y, 2)) <= VERTICES_LINKING_RANGE);
}
void Vertex::Select() {
	isSelected = true;
	selectedVertexID = id;
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
	UpdateInfoPanels();
	SetWindowTextA(VertexNameEditWnd, this->name.c_str());
}
void Vertex::Deselect() {
	isSelected = false;
	if (selectedVertexID == id)
		selectedVertexID = NULL;
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
	UpdateInfoPanels();
}
void Vertex::DeselectAll() {
	for (Vertex* v : vertices) 
	{
		v -> Deselect();
	}
	UpdateInfoPanels();
}

int Vertex::GetVertexIdx(UINT __id) {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i] -> GetID() == __id) {
			return i;
		}
	}
	return NULL;
}
Vertex* Vertex::GetVertex(UINT __id) {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i] -> GetID() == __id) {
			return vertices[i];
		}
	}
	return nullptr;
}
Vertex* Vertex::GetVertex(HWND __hWnd) {
	return GetVertex(GetWindowLongA(__hWnd, GWL_ID));
}

void Vertex::DeleteSelected() {
	Vertex::DeleteVertex(selectedVertexID);
}

void Vertex::DeleteVertex(UINT _id) {
	Vertex* v = Vertex::GetVertex(_id);
	for (Edge* e : Edge::edges)
		if (e->begin == v || e->end == v)
			Edge::DeleteEdge(e);
	int vec_idx = Vertex::GetVertexIdx(_id);
	DestroyWindow(v -> GetWindow());
	vertices.erase(vertices.begin() + vec_idx);
	selectedVertexID = NULL;
	UpdateInfoPanels();
}

RECT Vertex::GetRect()
{
	RECT r; 
	GetClientRect(hWnd, &r);
	return r;
}

void Vertex::DrawVertex(HDC _mDC)
{
	PALETTE pal = tmr->GetPalette();
	RECT r = GetRect();
	POINT C = GetCenter();
	vBrush = CreateSolidBrush(vRGB(pal.fbk));
	vPen = CreatePen(PS_SOLID, 10, vRGB(pal.fbk));
	HGDIOBJ oldp = SelectObject(_mDC, vPen);
	HGDIOBJ oldb = SelectObject(_mDC, vBrush);

	Rectangle(_mDC, 0, 0, r.right, r.bottom);

	vBrush = CreateSolidBrush(vRGB(pal.vbk));
	vPen = CreatePen(PS_SOLID, 10, vRGB(pal.vbd));
	DeleteObject(SelectObject(_mDC, vPen));
	DeleteObject(SelectObject(_mDC, vBrush));

	// Отрисуем все ДРУГИЕ соединения, проходящие через эту вершину
	vector<pair<UINT, vector<UINT>>> table = GetUniqueConnectionsTable();
	SelectObject(_mDC, linePen);
	if (cursorline != RECT{ } && prelinkedVertex == nullptr)
		DrawLine(_mDC, cursorline.left - C.x + 50, cursorline.top - C.y + 50,
			cursorline.right - C.x + 50, cursorline.bottom - C.y + 50);
	for (pair<UINT, vector<UINT>> para : table)
	{
		if (para.first == GetID()) continue;
		Vertex* v1 = GetVertex(para.first);
		POINT C1 = v1->GetCenter();
		for (UINT idx : para.second)
		{
			if (idx == GetID()) continue;
			Vertex* v2 = GetVertex(idx);
			POINT C2 = v2->GetCenter();
			DrawLine(_mDC, C1.x - C.x + 50, C1.y - C.y + 50, C2.x - C.x + 50, C2.y - C.y + 50);
		}
	}

	SelectObject(_mDC, vPen);
	Ellipse(_mDC, 12, 12, 88, 88);

	vPen = CreatePen(PS_SOLID, 5, vRGB(pal.vbd));
	DeleteObject(SelectObject(_mDC, vPen));
	SetTextColor(_mDC, vRGB(pal.text));
	SetBkColor(_mDC, vRGB(pal.vbk));

	Ellipse(_mDC, 5, 5, 95, 95);
	DrawTextA(_mDC, (GetName()).c_str(), -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// Если эта вершина является выбранной, нарисуем внутри синюю окружность
	if (IsSelected()) {
		vPen = CreatePen(PS_SOLID, 10, vRGB(pal.selection));
		vBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		DeleteObject(SelectObject(_mDC, vPen));
		DeleteObject(SelectObject(_mDC, vBrush));
		Ellipse(_mDC, 12, 12, 88, 88);
	}

	// Отрисуем все соединения С этой вершиной
	SelectObject(_mDC, linePen);
	for (UINT idx : connections)
	{
		Vertex* v2 = GetVertex(idx);
		POINT C2 = v2->GetCenter();
		POINT startv1 = intersectionPoints(POINT{ 50, 50 }, C2 - C + 50, POINT{ 50, 50 }, 48)[0];
		DrawLine(_mDC, startv1.x, startv1.y, C2.x - C.x + 50, C2.y - C.y + 50);
	}

	SelectObject(_mDC, oldp);
	SelectObject(_mDC, oldb);
	DeleteObject(vPen);
	DeleteObject(vBrush);
}

void Vertex::RedrawVertex()
{
	HDC							VDC;
	HDC							memDC;
	HBITMAP					memBM;
	RECT						r;

	// щя будет двойная буферизация

	GetClientRect(hWnd, &r);

	VDC = GetDC(hWnd);

	memDC = CreateCompatibleDC(VDC);
	memBM = CreateCompatibleBitmap(VDC, 100, 100);
	SelectObject(memDC, memBM);

	DrawVertex(memDC);

	BitBlt(VDC, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);

	ReleaseDC(hWnd, VDC);
	DeleteObject(memBM);
	DeleteDC(memDC);
}

void Vertex::UpdateInfoPanels() {

	// Если не выделена ни одна вершина, все поля должны быть пустыми
	if (!selectedVertexID) {
		// Сделаем edit переименования неактивным
		EnableWindow(VertexNameEditWnd, 0);
		// Проверим, не пустая ли она уже
		GetWindowTextA(VertexNameEditWnd, BUFFER, 30);
		if (string(BUFFER) != "")
			// Сделаем edit пустым
			SendMessageA(VertexNameEditWnd, WM_SETTEXT, NULL, (LPARAM)string("").c_str());

		if (!DeleteButtonWnd->IsDisabled())
			DeleteButtonWnd->Disable();

		if (WeightWnd->GetText() != "Вес: ")
			WeightWnd->SetText("Вес: ");
		return;
	}

	// Если выше не попали, значит выделена вершина, найдем ее
	Vertex* v = Vertex::GetSelected();
	
	// Сделаем активным поле переименования и вот всё что выше делали
	EnableWindow(VertexNameEditWnd, 1);

	if (DeleteButtonWnd->IsDisabled())
		DeleteButtonWnd->Enable();

	if (WeightWnd->GetText() != "Вес: " + to_string(v->GetWeight()))
		WeightWnd->SetText("Вес: " + to_string(v->GetWeight()));
}
void Vertex::UpdateCoordinates()
{
	if (!selectedVertexID)
	{
		GetWindowTextA(VertexXEditWnd, BUFFER, 30);
		if (string(BUFFER) != "") SetWindowTextA(VertexXEditWnd, "");

		GetWindowTextA(VertexYEditWnd, BUFFER, 30);
		if (string(BUFFER) != "") SetWindowTextA(VertexYEditWnd, "");

		EnableWindow(VertexYEditWnd, 0);
		EnableWindow(VertexXEditWnd, 0);
	}
	else
	{
		Vertex* v = Vertex::GetSelected();
		if (!isEditingX)
		{
			GetWindowTextA(VertexXEditWnd, BUFFER, 5);
			string x = to_string(v->GetPT().x);
			if (BUFFER != x) SetWindowTextA(VertexXEditWnd, x.c_str());
		}
		if (!isEditingY)
		{
			GetWindowTextA(VertexYEditWnd, BUFFER, 5);
			string y = to_string(v->GetPT().y);
			if (BUFFER != y) SetWindowTextA(VertexYEditWnd, y.c_str());
		}
		EnableWindow(VertexXEditWnd, 1);
		EnableWindow(VertexYEditWnd, 1);
	}
}

vector<pair<UINT, vector<UINT>>> Vertex::GetUniqueConnectionsTable()
{
	vector<pair<UINT, vector<UINT>>> table;
	for (Vertex* v : vertices)
	{
		table.push_back({ v->GetID(), { } });
		for (UINT vcon : v->GetConnections())
		{
			BOOL exists = FALSE;
			for (pair<UINT, vector<UINT>> para : table)
			{
				if (para.first == vcon)
				{
					exists = TRUE;
					break;
				}
			}
			if (exists) continue;
			table.back().second.push_back(vcon);
		}
	}
	return table;
}

Vertex* Vertex::GetSelected()
{
	return Vertex::GetVertex(selectedVertexID);
}


void Vertex::VertexRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Vertex::VertexWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = VERTEX_WC;
	RegisterClass(&wc);
}
void Vertex::VertexUnregister(void)
{
	UnregisterClass(VERTEX_WC, NULL);
}
//inline void Vertice::OnLeftMouseMove()
//{
//	
//}
//inline void Vertice::OnRightMouseMove()
//{
//
//}


LRESULT CALLBACK Vertex::VertexWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Vertex* v = Vertex::GetVertex(hWnd);

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

			v -> DrawVertex(memDC);

			BitBlt(VDC, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);

			EndPaint(hWnd, &vps);

			DeleteObject(memBM);
			DeleteDC(memDC);

			return 0;
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
					Vertex::DeselectAll();
				v -> Select();
			}

			break;
		}

		case WM_LBUTTONUP:
		{
			isLMBPressed = false;
			cursorline = { };
			POINT cursor = { };
			cursor.x = GET_X_LPARAM(lParam);
			cursor.y = GET_Y_LPARAM(lParam);
			for (Vertex* v1 : vertices)
			{
				if (prelinkedVertex != nullptr && v1 != v && v1->IsNear(cursor + v->GetPT()))
				{
					InvalidateRect(prelinkedVertex->GetWindow(), NULL, FALSE);
					UpdateWindow(prelinkedVertex->GetWindow());

					if (!isOrientedGraph)
					{
						UINT conres = v->Connect(prelinkedVertex->GetID());	// здесь создаю conres чтобы проверить,
						prelinkedVertex->Connect(v->GetID());							// удалось ли соединение
						Edge::edges.push_back(new Edge(v, prelinkedVertex));
						//MessageBoxA(hWnd, (conres == 0? "Ошибка соединения!" : "Соединены: " + v->GetName() + " и " + prelinkedVertex->GetName()).c_str(), "Попытка соединения вершин", MB_OK);
					}
					else
					{
						MessageBoxA(MainWnd, "У разраба еще ручки не дошли сделать ориентированный вариант графа", "Ошибка!", MB_OK);
					}
					prelinkedVertex = nullptr;
					break;
				}
			}
			ReleaseCapture();	
			FieldInstance.Redraw();
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

				if (dest.x <= 0 || dest.y <= 0 || dest.x + width >= parentRect.right || dest.y + length >= parentRect.bottom - 20) {
					break;
				}
				for (Vertex* v2 : vertices) {
					if (v  == v2) continue;
					POINT vpt = v2 -> GetPT();
					if (PointDistance(vpt, dest) > 100 + VERTICES_DISTANCE_ERROR) continue;
					else return DefWindowProc(hWnd, uMsg, wParam, lParam);
				}

				v -> SetPT(dest);
				MoveWindow(hWnd, v -> GetPT().x, v -> GetPT().y, 100, 100, FALSE);
				for (Edge* e : Edge::edges)
					if (e->begin == v || e->end == v)
						e->RecalcPosition();
				FieldInstance.Redraw();
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
				FieldInstance.DrawField(memFDC, FALSE);

				// Подготовим всё необходимое для рисования линии на ВЕРШИНЕ
				// Испульзуем двойную буферизацию для исключения мерцания
				RECT vr = v -> GetRect();
				HDC memVDC = CreateCompatibleDC(VDC);
				HBITMAP memVBM = CreateCompatibleBitmap(VDC, vr.right, vr.bottom);
				SelectObject(memVDC, memVBM);

				// Отрисуем линии на вершинах, пересекаемых линией до курсора
				POINT C = v->GetCenter();
				cursorline.left = C.x;
				cursorline.top = C.y;
				cursorline.right = (vloc + cursor).x;
				cursorline.bottom = (vloc + cursor).y;

				// Отрисуем вершину
				v -> DrawVertex(memVDC);

				// Выберем перо для рисования линии
				HGDIOBJ oldf = SelectObject(memFDC, linePen);
				HGDIOBJ old1 = SelectObject(memVDC, linePen);

				// Проверим возможность соединиться
				BOOL toNullPrelinkedVertex = TRUE;
				for (Vertex* v1 : vertices)
				{
					if (*v1 == *v) continue;
					/*OutputDebugStringA((to_string(cursor.x + vloc.x) + " - " + to_string(cursor.y + vloc.y) + "\t\t").c_str());
					OutputDebugStringA((to_string(v1.GetPT().x + 50) + " - " + to_string(v1.GetPT().y + 50) + "\t\t").c_str());
					OutputDebugStringA((to_string(v1.IsNear(cursor + vloc)) + "\n").c_str());*/
					if (v1 -> IsNear(cursor + vloc))
					{
						toNullPrelinkedVertex = FALSE;

						// Фикс бага отображения линии при быстрой смене фокуса
						if (prelinkedVertex != nullptr && prelinkedVertex != v1) 
						{
							/*InvalidateRect(prelinkedVertex->GetWindow(), NULL, FALSE);
							UpdateWindow(prelinkedVertex->GetWindow());*/
							prelinkedVertex->RedrawVertex();
						}

						// Запоминаем вершину на которой висит фокус (пригодится)
						prelinkedVertex = v1;

						// Получим координаты центра другой вершины
						POINT pt1 = v1 -> GetCenter();

						// Подготовим всё необходимое для рисования линии на ВЕРШИНЕ1
						// Испульзуем двойную буферизацию для исключения мерцания
						RECT vr1 = v1 -> GetRect();
						HDC VDC1 = GetDC(v1 -> GetWindow());
						HDC memVDC1 = CreateCompatibleDC(VDC1);
						HBITMAP memVBM1 = CreateCompatibleBitmap(VDC1, vr1.right, vr1.bottom);
						SelectObject(memVDC1, memVBM1);

						// Отрисуем другую вершину
						v1 -> DrawVertex(memVDC1);

						// Линия под вершинами (на окне FieldWnd)
						DrawLine(memFDC, vloc.x + 50, vloc.y + 50, pt1.x, pt1.y);										
						
						// Линия на первой вершине
						POINT startv = intersectionPoints(POINT{ 50, 50 }, pt1 - vloc, POINT{ 50, 50 }, 48)[0];
						DrawLine(memVDC, startv.x, startv.y, pt1.x - vloc.x, pt1.y - vloc.y);										
						
						// Линия на второй вершине
						HGDIOBJ old2 = SelectObject(memVDC1, linePen);
						POINT startv1 = intersectionPoints(POINT{ 50, 50 }, vloc - pt1 + 100, POINT{ 50, 50 }, 48)[0];
						DrawLine(memVDC1, startv1.x, startv1.y, vloc.x - pt1.x + 50 + 50, vloc.y - pt1.y + 50 + 50);		
						
						// Перенесем изображения
						BitBlt(FDC, 0, 0, fr.right, fr.bottom, memFDC, 0, 0, SRCCOPY);
						BitBlt(VDC, 0, 0, vr.right, vr.bottom, memVDC, 0, 0, SRCCOPY);
						BitBlt(VDC1, 0, 0, vr.right, vr.bottom, memVDC1, 0, 0, SRCCOPY);

						SelectObject(memFDC, oldf);
						SelectObject(memVDC, old1);
						SelectObject(memVDC1, old2);

						// Уничтожим использованные объекты
						ReleaseDC(FieldWnd, FDC);
						ReleaseDC(hWnd, VDC);
						ReleaseDC(v1 -> GetWindow(), VDC1);
						DeleteDC(memFDC);
						DeleteDC(memVDC);
						DeleteDC(memVDC1);
						DeleteBitmap(memFBM);
						DeleteBitmap(memVBM);
						DeleteBitmap(memVBM1);

						break;
					}
					else 
					{
						/*InvalidateRect(v1 -> GetWindow(), NULL, FALSE);
						UpdateWindow(v1 -> GetWindow());*/
						v1->RedrawVertex();
					}
				}
				if (toNullPrelinkedVertex) prelinkedVertex = nullptr;

				// Отрисуем линию
				DrawLine(memFDC, vloc.x + 50, vloc.y + 50, cursor.x + vloc.x, cursor.y + vloc.y);

				if (PointDistance(POINT{ 50, 50 }, cursor) > 48)
				{
					POINT start = intersectionPoints(POINT{ 50, 50 }, cursor, POINT{ 50, 50 }, 48)[0];
					DrawLine(memVDC, start.x, start.y, cursor.x, cursor.y);
				}

				// Перенесем изображения
				BitBlt(FDC, 0, 0, fr.right, fr.bottom, memFDC, 0, 0, SRCCOPY);
				BitBlt(VDC, 0, 0, vr.right, vr.bottom, memVDC, 0, 0, SRCCOPY);

				SelectObject(memFDC, oldf);
				SelectObject(memVDC, old1);

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

BOOL operator == (Vertex& _Left, Vertex& _Right)
{
	return (_Left.GetID() == _Right.GetID());
}