#include "Vertice.h"
#include "Field.h"
#include "Main.h"::GetLocalCoordinates


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
extern HWND VerticeNameWnd;
extern HWND TransformPositionWnd;
extern HWND WeightWnd;
extern HWND FieldWnd;
extern CHAR BUFFER[40];

int k = 0;



Vertice::Vertice(UINT _id, HWND _hWnd, POINT _pt) {
	id = _id;
	hWnd = _hWnd;
	pt = _pt;
	weight = 0;
	name = to_string(_id-100);
	isSelected = false;
	isValid = true;
	hdc = GetDC(hWnd);
}

Vertice::Vertice() {
	isValid = false;
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

void Vertice::SetHDC(HDC _hdc) {
	hdc = _hdc;
}
HDC* Vertice::GetHDC() {
	return &hdc;
}

BOOL Vertice::IsSelected() {
	return isSelected;
}
void Vertice::Select() {
	isSelected = true;
	selectedVerticeID = id;
	InvalidateRect(hWnd, NULL, RDW_ERASE);
	UpdateWindow(hWnd);
	UpdateInfoPanels();
}
void Vertice::Deselect() {
	isSelected = false;
	if (selectedVerticeID == id)
		selectedVerticeID = NULL;
	InvalidateRect(hWnd, NULL, RDW_ERASE);
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
	Vertice& v = *Vertice::GetVertice(_id);
	int vec_idx = Vertice::GetVerticeIdx(_id);
	DestroyWindow(v.GetWindow());
	vertices.erase(vertices.begin() + vec_idx);
	selectedVerticeID = NULL;
	UpdateInfoPanels();
}


void Vertice::UpdateInfoPanels() {
	if (!selectedVerticeID) {
		
		GetWindowTextA(VerticeNameWnd, BUFFER, 30);
		if (string(BUFFER) != "Обознечение: ")
			SendMessageA(VerticeNameWnd, WM_SETTEXT, NULL, (LPARAM)string("Обознечение: ").c_str());
		
		GetWindowTextA(TransformPositionWnd, BUFFER, 30);
		if (string(BUFFER) != "Позиция: ")
			SendMessageA(TransformPositionWnd, WM_SETTEXT, NULL, (LPARAM)string("Позиция: ").c_str());
		
		GetWindowTextA(WeightWnd, BUFFER, 30);
		if (string(BUFFER) != "Вес: ")
			SendMessageA(WeightWnd, WM_SETTEXT, NULL, (LPARAM)string("Вес: ").c_str());
		return;
	}

	Vertice &v = *Vertice::GetVertice(selectedVerticeID);

	GetWindowTextA(VerticeNameWnd, BUFFER, 30);
	if (string(BUFFER) != "Обознечение: " + v.GetName())
		SendMessageA(VerticeNameWnd, WM_SETTEXT, NULL, (LPARAM)string("Обознечение: " + v.GetName()).c_str());

	GetWindowTextA(TransformPositionWnd, BUFFER, 30);
	if (string(BUFFER) != "Позиция: (" + to_string(v.GetPT().x) + "; " + to_string(v.GetPT().y) + ")")
		SendMessageA(TransformPositionWnd, WM_SETTEXT, NULL, (LPARAM)string("Позиция: (" + to_string(v.GetPT().x) + "; " + to_string(v.GetPT().y) + ")").c_str());

	GetWindowTextA(WeightWnd, BUFFER, 30);
	if (string(BUFFER) != "Вес: " + to_string(v.GetWeight()))
		SendMessageA(WeightWnd, WM_SETTEXT, NULL, (LPARAM)string("Вес: " + to_string(v.GetWeight())).c_str());
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
	Vertice &v = *Vertice::GetVertice(hWnd);

	switch (uMsg) {

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC memDC;
			HPEN hPen;
			HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
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
			DrawTextA(memDC, (v.GetName()).c_str(), -1, &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			// Если эта вершина является выбранной
			if (v.IsSelected()) {
				hPen = CreatePen(PS_SOLID, 10, RGB(100, 149, 237));
				SelectObject(memDC, hPen);
				SelectObject(memDC, hBrush);
				Ellipse(memDC, 12, 12, 88, 88);
			}

			BitBlt(*v.GetHDC(), 0, 0, r.right, r.bottom, memDC, 0, 0, SRCCOPY);

			EndPaint(hWnd, &ps);

			DeleteObject(hPen);
			DeleteObject(hBrush);
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
			RECT rect;
			GetClientRect(hWnd, &rect);
			InvalidateRect(hWnd, &rect, TRUE);
			UpdateWindow(hWnd);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			isLMBPressed = true;
			SetCapture(hWnd);
			if (v.IsSelected()) {
					v.Deselect();
			}
			else {
				if (selmode == mode1)
					Vertice::DeselectAll();
				v.Select();
			}

			break;
		}

		case WM_LBUTTONUP:
		{
			isLMBPressed = false;
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
					if (v.GetID() == v2.GetID()) continue;
					POINT vpt = v2.GetPT();
					if (sqrt(pow(abs(vpt.x - dest.x), 2) + pow(abs(vpt.y - dest.y), 2)) > 100 + VERTICE_DISTANCE_ERROR) continue;
					else return DefWindowProc(hWnd, uMsg, wParam, lParam);;
				}


				v.SetPT(dest);

				/*InvalidateRect(FieldWnd, GetLocalRect(hWnd), FALSE);
				UpdateWindow(FieldWnd);*/

				MoveWindow(hWnd, v.GetPT().x, v.GetPT().y, 100, 100, TRUE);
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