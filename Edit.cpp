#include "Edit.h"

map<HWND, Edit*> Edit::objmap = { };

Edit::Edit(HWND _hParWnd, string _UnfocusedText, V3 _Position, V3 _Size, EDITPARAMS _Params)
{
	unfoctext = _UnfocusedText;
	params = _Params;
	transform.position = _Position;
	transform.size = _Size;
	wnd = GenWnd(_hParWnd);
	objmap[wnd] = this;
	state = inactive;
}

HWND Edit::GenWnd(HWND _hParWnd)
{
	wnd = CreateWindow(EDIT_WC, L"name", WS_CHILD | WS_VISIBLE,
		transform.position.x, transform.position.y, transform.size.x, transform.size.y, _hParWnd, NULL, NULL, NULL);
	return wnd;
}

void Edit::CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void Edit::TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam == REDRAW_IDT) this->Redraw();
}

void Edit::Redraw()
{
	HDC hDC = GetDC(wnd);
	HDC mDC = CreateCompatibleDC(hDC);
	HBITMAP mBM = CreateCompatibleBitmap(hDC, transform.size.x, transform.size.y);
	HBRUSH hBrush = CreateSolidBrush(vRGB(MAIN_BK_COL));
	HPEN hPen = CreatePen(BS_SOLID, 0, vRGB(MAIN_BK_COL));
	SelectObject(mDC, mBM);

	HGDIOBJ oldb = SelectObject(mDC, hBrush);
	HGDIOBJ oldp = SelectObject(mDC, hPen);
	Rectangle(mDC, 0, 0, transform.size.x, transform.size.y);

	switch(state)
	{
		case inactive:
			hBrush = CreateSolidBrush(vRGB(params.bkCol));
			hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdDefCol));
			break;
		case hovered:
			hBrush = CreateSolidBrush(vRGB(params.bkHovCol));
			hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdHovCol));
			break;
		case focused:
			hBrush = CreateSolidBrush(vRGB(params.bkFocCol));
			hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdFocCol));
			break;
	}

	DeleteObject(SelectObject(mDC, hBrush));
	DeleteObject(SelectObject(mDC, hPen));

	RoundRect(mDC, 2, 2, transform.size.x - 2, transform.size.y - 2, 13, 13);

	BitBlt(hDC, 0, 0, transform.size.x, transform.size.y, mDC, 0, 0, SRCCOPY);

	SelectObject(mDC, oldp);
	SelectObject(mDC, oldb);

	ReleaseDC(wnd, hDC);
	DeleteDC(mDC);
	DeleteBitmap(mBM);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(oldp);
	DeleteObject(oldb);
}

void Edit::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, REDRAW_IDT, REDRAW_RATE, NULL);
}

LRESULT Edit::EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	EDIT* obj = objmap[hWnd];

	switch (uMsg)
	{
		case WM_COMMAND:
		{
			if (obj) obj->CommandHandler(hWnd, wParam, lParam);
			break;
		}
		case WM_TIMER:
		{
			if (obj) obj->TimerManager(hWnd, wParam, lParam);
			break;
		}
		case WM_CREATE:
		{
			OnCreate(hWnd, wParam, lParam);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (obj)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);

				if (obj->state != focused) obj->state = hovered;
			}
			break;
		}
		case WM_MOUSELEAVE:
		{
			if (obj)
			{
				if (obj->state != focused) obj->state = inactive;
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (obj)
			{
				if (obj -> state == hovered)
				{
					obj->state = focused;
					obj->params.bdWidth++;
				}
				else 
				{
					obj->state = hovered;
					obj->params.bdWidth--;
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{

			break;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
