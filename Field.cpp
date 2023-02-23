#include "Field.h"
#include "Vertice.h"

extern Field FieldInstance;
extern vector<Vertice> vertices;

Field::Field(HWND _hWnd) {
	hWnd = _hWnd;
};

void Field::SetWindow(HWND& _hWnd) {
	hWnd = _hWnd;
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

			GetClientRect(hWnd, &r);

			hdc = BeginPaint(hWnd, &ps);

			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			HBRUSH hBrush = CreateSolidBrush(HOLLOW_BRUSH);
			Rectangle(hdc, 0, 0, r.right, r.bottom);

			EndPaint(hWnd, &ps);

			break;
		}

		case WM_LBUTTONDOWN:
		{
			POINT pt = {};
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			RECT r;
			GetClientRect(hWnd, &r);
			if (vertices.size() < 30 && pt.x > 50 && pt.x < r.right - 50 && pt.y > 50 && pt.y < r.bottom - 50)
			{
				for (Vertice v : vertices) {
					POINT vpt = v.GetPT();
					if (sqrt(pow(abs(vpt.x - pt.x), 2) + pow(abs(vpt.y - pt.y), 2)) > 100) continue;
					else return 0;
				}
				UINT new_id = Vertice::GenerateID();
				vertices.push_back(Vertice(new_id, CreateWindow(VERTICE_WC, NULL, WS_CHILD | WS_VISIBLE, pt.x - 50, pt.y - 50, 100, 100, hWnd, (HMENU)new_id, NULL, NULL), pt));
				//OutputDebugStringA("Вершина создана!\n");
			}
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