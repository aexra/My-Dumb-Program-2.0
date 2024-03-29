#include "Flag.h"
#include "ThemeManager.h"

map<HWND, Flag*> Flag::objmap = { };
extern ThemeManager* tmr;

Flag::Flag(HWND _hParWnd, string _UnfocusedText, V3 _Position, V3 _Size, FLAGPARAMS _Params)
{
	unfoctext = _UnfocusedText;
	params = _Params;
	transform.position = _Position;
	transform.size = _Size;
	wnd = GenWnd(_hParWnd);
	objmap[wnd] = this;
	state = inactive;
}

HWND Flag::GenWnd(HWND _hParWnd)
{
	wnd = CreateWindow(FLAG_WC, L"name", WS_CHILD | WS_VISIBLE,
		transform.position.x, transform.position.y, transform.size.x, transform.size.y, _hParWnd, NULL, NULL, NULL);
	return wnd;
}

void Flag::CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void Flag::TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam == REDRAW_IDT)
	{
		if (laststate != state)
		{
			laststate = state;
			Invalidate();
		}
	}
}

void Flag::Redraw()
{
	PALETTE plt = tmr->GetPalette();
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(wnd, &ps);
	HDC mDC = CreateCompatibleDC(hDC);
	HBITMAP mBM = CreateCompatibleBitmap(hDC, transform.size.x, transform.size.y);
	HBRUSH hBrush = CreateSolidBrush(vRGB(plt.fbk));
	HPEN hPen = CreatePen(BS_SOLID, 0, vRGB(plt.fbk));
	SelectObject(mDC, mBM);

	HGDIOBJ oldb = SelectObject(mDC, hBrush);
	HGDIOBJ oldp = SelectObject(mDC, hPen);
	Rectangle(mDC, 0, 0, transform.size.x, transform.size.y);

	switch (state)
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

	EndPaint(wnd, &ps);
	DeleteDC(mDC);
	DeleteBitmap(mBM);
	DeleteObject(hBrush);
	DeleteObject(hPen);
	DeleteObject(oldp);
	DeleteObject(oldb);
}

void Flag::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, REDRAW_IDT, REDRAW_RATE, NULL);
}

LRESULT Flag::FlagProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLAG* obj = objmap[hWnd];

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (obj) obj->CommandHandler(hWnd, wParam, lParam);
		break;
	}
	case WM_PAINT:
	{
		obj->Redraw();
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
			if (obj->state == hovered)
			{
				obj->state = focused;
				obj->params.bdWidth++;
				obj->is = true;
			}
			else
			{
				obj->state = hovered;
				obj->params.bdWidth--;
				obj->is = false;
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
