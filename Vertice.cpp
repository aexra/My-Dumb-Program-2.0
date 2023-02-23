#include "Vertice.h"


extern vector<Vertice> vertices;


Vertice::Vertice(UINT _id, HWND _hWnd, POINT _pt) {
	id = _id;
	hWnd = _hWnd;
	pt = _pt;
	weight = 0;
	name = to_string(_id);
	isSelected = false;
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


void Vertice::Select() {

}
void Vertice::Deselect() {

}
void Vertice::DeselectAll() {

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


LRESULT CALLBACK Vertice::VerticeWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			HPEN hPen;
			RECT r;

			//InvalidateRect(hWnd, NULL, RDW_ERASE);

			GetClientRect(hWnd, &r);
			hdc = BeginPaint(hWnd, &ps);

			hPen = CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
			SelectObject(hdc, hPen);
			Ellipse(hdc, 12, 12, 88, 88);

			hPen = CreatePen(PS_SOLID, 5, RGB(89, 89, 89));
			SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
			SelectObject(hdc, hPen);
			SetTextColor(hdc, RGB(0, 0, 0));

			Ellipse(hdc, 5, 5, 95, 95);
			DrawTextA(hdc, (std::to_string(GetWindowLongA(hWnd, GWL_ID) - 100) + "\n").c_str(), -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			EndPaint(hWnd, &ps);

			DeleteObject(hPen);

			break;
		}

		case WM_CREATE:
		{
			//OutputDebugStringA("Вершина реально создана!\n");
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