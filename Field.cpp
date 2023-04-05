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
void Field::DrawField(HDC _mDC)
{
	HGDIOBJ orig = SelectObject(_mDC, fPen);
	Rectangle(_mDC, 0, 0, rect.right, rect.bottom);
}
void Field::DrawConnection(Vertex* _V1, Vertex* _V2)
{
	POINT C1 = _V1->GetCenter();
	POINT C2 = _V2->GetCenter();
	RECT r1 = _V1->GetRect();
	RECT r2 = _V2->GetRect();
	HDC VDC1 = GetDC(_V1->GetWindow());
	HDC VDC2 = GetDC(_V2->GetWindow());
	HDC FDC = GetDC(hWnd);
	HDC mVDC1 = CreateCompatibleDC(VDC1);
	HDC mVDC2 = CreateCompatibleDC(VDC2);
	HDC mFDC = CreateCompatibleDC(FDC);
	HBITMAP mVBM1 = CreateCompatibleBitmap(VDC1, 100, 100);
	HBITMAP mVBM2 = CreateCompatibleBitmap(VDC2, 100, 100);
	HBITMAP mFBM = CreateCompatibleBitmap(FDC, rect.right, rect.bottom);
	SelectObject(mVDC1, mVBM1);
	SelectObject(mVDC2, mVBM2);
	SelectObject(mFDC, mFBM);

	_V1->DrawVertex(mVDC1);
	_V2->DrawVertex(mVDC2);
	DrawField(mFDC);

	HGDIOBJ old1 = SelectObject(mVDC1, linePen);
	HGDIOBJ old2 = SelectObject(mVDC2, linePen);
	HGDIOBJ oldf = SelectObject(mFDC, linePen);

	POINT startv1 = intersectionPoints(POINT{ 50, 50 }, C2 - C1 + 50, POINT{ 50, 50 }, 48)[0];
	DrawLine(mVDC1, startv1.x, startv1.y, C2.x - C1.x + 50, C2.y - C1.y + 50);

	POINT startv2 = intersectionPoints(POINT{ 50, 50 }, C1 - C2 + 50, POINT{ 50, 50 }, 48)[0];
	DrawLine(mVDC2, startv2.x, startv2.y, C1.x - C2.x + 50, C1.y - C2.y + 50);

	DrawLine(mFDC, C1.x, C1.y, C2.x, C2.y);

	SelectObject(mVDC1, old1);
	SelectObject(mVDC2, old2);
	SelectObject(mFDC, oldf);

	BitBlt(FDC, 0, 0, rect.right, rect.bottom, mFDC, 0, 0, SRCCOPY);
	BitBlt(VDC1, 0, 0, r1.right, r1.bottom, mVDC1, 0, 0, SRCCOPY);
	BitBlt(VDC2, 0, 0, r2.right, r2.bottom, mVDC2, 0, 0, SRCCOPY);

	ReleaseDC(_V1->GetWindow(), VDC1);
	ReleaseDC(_V2->GetWindow(), VDC2);
	ReleaseDC(hWnd, FDC);
	DeleteDC(mVDC1);
	DeleteDC(mVDC2);
	DeleteDC(mFDC);
	DeleteBitmap(mVBM1);
	DeleteBitmap(mVBM2);
	DeleteBitmap(mFBM);
}
void Field::Redraw()
{
	HDC hDC = GetDC(hWnd);
	HDC mDC = CreateCompatibleDC(hDC);
	HBITMAP mBM = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
	SelectObject(hDC, mBM);

	DrawField(mDC);

	BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

	ReleaseDC(hWnd, hDC);
	DeleteDC(mDC);
	DeleteBitmap(mBM);
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
			//FillRect(hdc, &r, hBrush);
			Rectangle(hdc, 0, 0, r.right, r.bottom);

			EndPaint(hWnd, &ps);

			DeleteObject(hBrush);
			DeleteObject(hPen);
			break;
		}

		case WM_ERASEBKGND:
		{
			return 0;
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
			if (vertices.size() < MAX_VERTICES && FieldInstance.IsPtInBorders(pt))
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