#include "Field.h"
#include "Vertice.h"
#include "Main.h"

extern Field FieldInstance;
extern vector<Vertice> vertices;
extern UINT selectedVerticeID;
extern BOOL isRMBPressed;
extern BOOL isLMBPressed;
extern HPEN linePen;

Field::Field(HWND _hWnd) {
	hWnd = _hWnd;
};

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
HDC Field::GetHDC()
{
	return GetDC(hWnd);
}

// TRUE is returned when the collision is detected and FALSE is returned when not
// pt is a POINT to coordinates of the chosen vertice
BOOL	Field::CheckVerticeCollisions(POINT _pt) {
	for (Vertice v : vertices) {
		POINT vpt = v.GetPT();
		if (sqrt(pow(abs(vpt.x - _pt.x), 2) + pow(abs(vpt.y - _pt.y), 2)) > 100 + VERTICE_DISTANCE_ERROR) continue;
		else return 1;
	}
	return 0;
}
BOOL Field::IsPtInBorders(POINT _pt) {
	RECT r = GetRect();
	BOOL result = (_pt.x > 60 && _pt.x < r.right - 60 && _pt.y > 60 && _pt.y < r.bottom - 60);
	return result;
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

			DeleteObject(hdc);
			DeleteObject(hBrush);
			DeleteObject(hPen);
			break;
		}

		case WM_ERASEBKGND:
		{
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			POINT pt = {};
			pt.x = GET_X_LPARAM(lParam) - 50;
			pt.y = GET_Y_LPARAM(lParam) - 50;
			if (vertices.size() < MAX_VERTICE && FieldInstance.IsPtInBorders(pt))
			{
				if (FieldInstance.CheckVerticeCollisions(pt)) return 0;
				UINT new_id = Vertice::GenerateID();
				vertices.push_back(Vertice(new_id, CreateWindow(VERTICE_WC, NULL, WS_CHILD | WS_VISIBLE, pt.x, pt.y, 100, 100, hWnd, (HMENU)new_id, NULL, NULL), pt));
			}
			break;
		}

		case WM_MOUSEMOVE:
		{
			if (isLMBPressed && selectedVerticeID)
			{
				Vertice& v = *Vertice::GetVertice(selectedVerticeID);
				POINT cursor = { };
				POINT vloc = v.GetPT();

				cursor.x = GET_X_LPARAM(lParam);
				cursor.y = GET_Y_LPARAM(lParam);
				
				HDC FDC = GetDC(hWnd);
				


				SelectObject(FDC, linePen);
				SelectObject(*v.GetHDC(), linePen);
				DrawLine(FDC, vloc.x+50, vloc.y+50, cursor.x, cursor.y);
				DrawLine(*v.GetHDC(), 0, 0, cursor.x, cursor.y);
				
				ReleaseDC(hWnd, FDC);
			}

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
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}