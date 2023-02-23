#include "Field.h"

extern Field FieldInstance;

Field::Field(HWND _hWnd) {
	hWnd = _hWnd;
};

void Field::SetWindow(HWND& _hWnd) {
	hWnd = _hWnd;
}

void Field::FieldRegister(void) {
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = FieldInstance.FieldWndProc;
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
			OutputDebugStringA("LMB Pressed\n");
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