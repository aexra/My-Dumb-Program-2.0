﻿#include "Vertice.h"
#include "Field.h"
#include "Main.h"::GetLocalCoordinates


extern vector<Vertice> vertices;
extern UINT selectedVerticeID;
extern BOOL isRMBPressed;
extern BOOL isLMBPressed;
extern Field FieldInstance;
extern enum selection_mode {mode1, mode2};
extern selection_mode selmode;


Vertice::Vertice(UINT _id, HWND _hWnd, POINT _pt) {
	id = _id;
	hWnd = _hWnd;
	pt = _pt;
	weight = 0;
	name = to_string(_id);
	isSelected = false;
	isValid = true;
}

Vertice::Vertice() {
	isValid = false;
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

FLOAT	 Vertice::GetWeight() {
	return weight;
}
FLOAT	 Vertice::SetWeight(FLOAT _weight) {
	weight = _weight;
	return weight;
}


POINT	 Vertice::GetPT() {
	return pt;
}
POINT	 Vertice::SetPT(POINT _pt) {
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


// TODO
vector<UINT> Vertice::GetConnections() {
	return connections;
}
vector<UINT> Vertice::SetConnections(vector<UINT> cons) {
	return connections;
}


// TODO
UINT Vertice::Connect(UINT _id) {
	return _id;
}
UINT Vertice::Disconnect(UINT _id) {
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
void Vertice::Select() {
	isSelected = true;
	UpdateWindow(hWnd);
	UpdateInfoPanels();
}
void Vertice::Deselect() {
	isSelected = false;
	UpdateWindow(hWnd);
	UpdateInfoPanels();
}
void Vertice::DeselectAll() {
	for (int i = 0; i < vertices.size(); i++) if (vertices[i].IsSelected()) vertices[i].Deselect();
}

int Vertice::GetVerticeIdx(UINT __id) {

}
Vertice& Vertice::GetVertice(UINT __id) {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].GetID() == __id) {
			return vertices[i];
		}
	}
}

void Vertice::DeleteSelected() {

}


void Vertice::UpdateInfoPanels() {

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


HDC hdc = { };
LRESULT CALLBACK Vertice::VerticeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	switch (uMsg) {

		//case WM_ERASEBKGND:
		//{
		//	MakeItWhite(hWnd, hdc);
		//	break;
		//}


		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC memDC;
			HPEN hPen;
			RECT r;

			// щя будет двойная буферизация

			GetClientRect(hWnd, &r);
			memDC = BeginPaint(hWnd, &ps);

			hPen = CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
			SelectObject(memDC, hPen);
			Ellipse(memDC, 12, 12, 88, 88);

			hPen = CreatePen(PS_SOLID, 5, RGB(89, 89, 89));
			SelectObject(memDC, GetStockObject(HOLLOW_BRUSH));
			SelectObject(memDC, hPen);
			SetTextColor(memDC, RGB(0, 0, 0));

			Ellipse(memDC, 5, 5, 95, 95);
			DrawTextA(memDC, (std::to_string(GetWindowLongA(hWnd, GWL_ID) - 100) + "\n").c_str(), -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			// Если эта вершина является выбранной
			if (Vertice::GetVertice(GetWindowLongA(hWnd, GWL_ID)).IsSelected()) {
				hPen = CreatePen(PS_SOLID, 10, RGB(100, 149, 237));
				SelectObject(memDC, hPen);
				SelectObject(memDC, GetStockObject(HOLLOW_BRUSH));

				Ellipse(memDC, 12, 12, 88, 88);
			}

			BitBlt(hdc, 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);

			EndPaint(hWnd, &ps);

			DeleteObject(hPen);
			DeleteObject(memDC);

			break;
		}

		case WM_NCHITTEST: 
		{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			LRESULT hit = DefWindowProc(hWnd, uMsg, wParam, lParam);
			if (hit == HTCLIENT)
				if (FieldInstance.IsPtInBorders(GetLocalCoordinates(hWnd))) hit = HTCAPTION;
			return hit;
		}

		case WM_LBUTTONDOWN:
		{
			isLMBPressed = true;
			//SetCapture(hWnd);
			Vertice v = GetVertice(GetWindowLongA(hWnd, GWL_ID));
			if (v.IsSelected()) {
				if (selmode == mode1) {
					Vertice::DeselectAll();
				} else {
					v.Deselect();
				}
			}
			else {
				OutputDebugStringA("just selected\n");
				v.Select();
			}

			break;
		}

		case WM_LBUTTONUP:
		{
			isLMBPressed = false;
			//ReleaseCapture();
			break;
		}

		case WM_CREATE:
		{
			hdc = GetDC(hWnd);
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