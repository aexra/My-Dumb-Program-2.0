#include "Field.h"
#include "Vertex.h"
#include "Main.h"
#include "ExtraOverloads.h"

extern Field						FieldInstance;
extern vector<Vertex*>	vertices;
extern UINT					selectedVertexID;
extern BOOL					isRMBPressed;
extern BOOL					isLMBPressed;
extern HPEN					linePen;

HPEN								fPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
HBRUSH							fBrush = CreateSolidBrush(RGB(255, 255, 255));

Field::Field(HWND _hWnd) {
	hWnd = _hWnd;
	GetClientRect(hWnd, &rect);
}

void Field::SetWindow(HWND& _hWnd) {
	hWnd = _hWnd;
}

RECT Field::GetRect() {
	return rect;
}
RECT Field::SetRect(RECT _rect) {
	rect = _rect;
	return rect;
}

// TRUE is returned when the collision is detected and FALSE is returned when not
// pt is a POINT to coordinates of the chosen vertice
BOOL	Field::CheckVertexCollision(const POINT& _pt) {
	for (Vertex* v : vertices) {
		POINT vpt = v -> GetPT();
		if (sqrt(pow(abs(vpt.x - _pt.x), 2) + pow(abs(vpt.y - _pt.y), 2)) > 100 + VERTICES_DISTANCE_ERROR) continue;
		else return 1;
	}
	return 0;
}
BOOL Field::IsPtInBorders(const POINT& _pt) {
	RECT r = GetRect();
	BOOL result = (_pt.x > 60 && _pt.x < r.right - 60 && _pt.y > 60 && _pt.y < r.bottom - 60);
	return result;
}
void Field::DrawField(HDC _mDC, BOOL _RedrawVertices)
{
	HGDIOBJ orig = SelectObject(_mDC, fPen);
	Rectangle(_mDC, 0, 0, rect.right+1, rect.bottom+1);

	vector<pair<UINT, vector<UINT>>> table = Vertex::GetUniqueConnectionsTable();

	if (_RedrawVertices)
	{
		for (Vertex* v : vertices)
		{
			v->RedrawVertex();
		}
	}

	HGDIOBJ oldf = SelectObject(_mDC, linePen);
	for (pair<UINT, vector<UINT>> vpair : table)
	{
		Vertex* v1 = Vertex::GetVertex(vpair.first);
		POINT C1 = v1->GetCenter();

		for (UINT vid2 : vpair.second)
		{
			Vertex* v2 = Vertex::GetVertex(vid2);
			POINT C2 = v2->GetCenter();
			DrawLine(_mDC, C1.x, C1.y, C2.x, C2.y);
			
		}
	}
	SelectObject(_mDC, oldf);
}

void Field::Redraw(BOOL _RedrawVertices)
{
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

void Field::FieldRegister(void) {
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Field::FieldWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = FIELD_WC;
	RegisterClass(&wc);
}
void Field::FieldUnregister() {
	UnregisterClass(FIELD_WC, NULL);
}

LRESULT CALLBACK Field::FieldWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

		case WM_PAINT:
		{
			HDC hdc;
			RECT r;
			PAINTSTRUCT ps;
			HBRUSH hBrush;
			HPEN hPen;

			GetClientRect(hWnd, &r);

			hdc = BeginPaint(hWnd, &ps);

			hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			
			HDC mDC = CreateCompatibleDC(hdc);
			HBITMAP mBM = CreateCompatibleBitmap(hdc, r.right, r.bottom);
			SelectObject(mDC, mBM);

			FieldInstance.DrawField(mDC);

			BitBlt(hdc, 0, 0, r.right, r.bottom, mDC, 0, 0, SRCCOPY);

			EndPaint(hWnd, &ps);

			DeleteObject(hBrush);
			DeleteObject(hPen);
			DeleteDC(mDC);
			DeleteBitmap(mBM);
			break;
		}

		case WM_ERASEBKGND:
		{
			return 1;
		}

		/*case WM_TIMER:
		{
			switch (wParam)
			{
				case FIELD_REFRESH_IDT:
				{
					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);
					break;
				}
			}
			break;
		}*/

		case WM_LBUTTONDOWN:
		{
			POINT pt = {};
			pt.x = GET_X_LPARAM(lParam) - 50;
			pt.y = GET_Y_LPARAM(lParam) - 50;
			if (vertices.size() < MAX_VERTICES && FieldInstance.IsPtInBorders(pt+50))
			{
				if (FieldInstance.CheckVertexCollision(pt)) return 0;
				UINT new_id = Vertex::GenerateID();
				vertices.push_back(new Vertex(new_id, CreateWindow(VERTEX_WC, NULL, WS_CHILD | WS_VISIBLE, pt.x, pt.y, 100, 100, hWnd, (HMENU)new_id, NULL, NULL), pt));
			}
			break;
		}

		case WM_LBUTTONUP:
		{

			break;
		}

		case WM_MOUSEMOVE:
		{
			
			break;
		}

		case WM_RBUTTONDOWN:
		{
			isRMBPressed = true;
			break;
		}

		case WM_RBUTTONUP:
		{
			isRMBPressed = false;
			break;
		}

		case WM_DESTROY:
		{
			DestroyWindow(hWnd);
			break;
		}

		case WM_CREATE:
		{
			RECT r;
			GetClientRect(hWnd, &r);
			FieldInstance.SetRect(r);
			//SetTimer(hWnd, FIELD_REFRESH_IDT, FIELD_REFRESH_RATE, NULL);
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}