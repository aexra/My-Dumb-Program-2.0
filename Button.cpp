#include "Button.h"

map<HWND, Button*> Button::objmap = { };
extern BOOL isLMBPressed_LIBVAR;
extern WNDPROC defaultStaticProc_LIBVAR;

Button::Button(HWND _hParWnd, string _Text, V3 _Position, UINT_PTR _nIDEvent, V3 _Size, BUTTONPARAMS _Params)
{
	text = _Text;
	params = _Params;
	params.bdWidth = max(_Size.z, 3);
	transform.position = _Position;
	nIDEvent = _nIDEvent;
	transform.size = _Size;
	state = params.startstate;
	laststate = invalid;
	GenWnd(_hParWnd);
	objmap[wnd] = this;
}

void Button::GenWnd(HWND _hParWnd)
{
	wnd = CreateWindow(BUTTON_WC, L"name", WS_CHILD | WS_VISIBLE,
		transform.position.x, transform.position.y, transform.size.x, transform.size.y, _hParWnd, NULL, NULL, NULL);
	placeholder = CreateWindowA("static", text.c_str(), WS_CHILD | WS_VISIBLE |
		(params.alignh == aligns::center ? SS_CENTER : params.alignh == aligns::left ? SS_LEFT : SS_RIGHT) | 
		(params.alignv == aligns::center ? BS_CENTER : params.alignv == aligns::top ? BS_TOP : BS_BOTTOM),
		7, 7, transform.size.x - 14, transform.size.y - 14, wnd, NULL, NULL, NULL);
	defaultStaticProc_LIBVAR = (WNDPROC)GetWindowLongPtrA(placeholder, GWLP_WNDPROC);
	SetWindowLongPtrA(placeholder, GWLP_WNDPROC, (LONG_PTR)PlaceholderProc);
	HFONT hf = CreateFont(params.charHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Comic Sans MS");
	SendMessage(placeholder, WM_SETFONT, (WPARAM)hf, 0);
}

BOOL Button::MoveControl(V3 _To)
{
	transform.Translate(_To);
	MoveWindow(wnd, _To.x, _To.y, transform.size.x, transform.size.y, TRUE);
	MoveWindow(placeholder, _To.x + 7, _To.y + 7, transform.size.x-14, transform.size.y-14, TRUE);
	return 1; 
}

void Button::Disable()
{
	state = disabled;
}

void Button::Enable()
{
	state = enabled;
}

void Button::Show()
{
	ShowWindow(wnd, 1);
	ShowWindow(placeholder, 1);
}

void Button::Hide()
{
	ShowWindow(wnd, 0);
	ShowWindow(placeholder, 0);
}

void Button::Press()
{
	state = pressed;
	SetTimer(wnd, UNPRESS_IDT, UNPRESS_DURATION, NULL);
	SendMessageA(GetParent(wnd), WM_COMMAND, nIDEvent, NULL);
}

void Button::CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void Button::TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	BUTTON* obj = objmap[hWnd];
	if (wParam == REDRAW_IDT) this->Redraw();
	else if (wParam == UNPRESS_IDT)
	{
		if (isLMBPressed_LIBVAR) return;
		RECT r = { };
		POINT pt = { };
		GetCursorPos(&pt);
		GetWindowRect(hWnd, &r);
		if (PtInRect(&r, pt))
			obj->state = hovered;
		else obj->state = enabled;
		KillTimer(hWnd, UNPRESS_IDT);
		//MessageBoxA(NULL, (to_string(pt.x) + " - " + to_string(pt.y) + " - " + to_string(r.left) + " - " + to_string(r.top)).c_str(), "cap", MB_OK);
	}
}

void Button::Redraw()
{
	if (laststate == state) return;
	else laststate = state;

	HDC hDC = GetDC(wnd);
	HDC mDC = CreateCompatibleDC(hDC);
	HBITMAP mBM = CreateCompatibleBitmap(hDC, transform.size.x, transform.size.y);
	HBRUSH hBrush = CreateSolidBrush(vRGB(MAIN_BK_COL));
	HPEN hPen = CreatePen(BS_SOLID, 0, vRGB(MAIN_BK_COL));
	SelectObject(mDC, mBM);

	HGDIOBJ oldb = SelectObject(mDC, hBrush);
	HGDIOBJ oldp = SelectObject(mDC, hPen);
	Rectangle(mDC, 0, 0, transform.size.x, transform.size.y);

	SetWindowLongA(placeholder, GWL_STYLE, WS_CHILD | WS_VISIBLE |
		(params.alignh == aligns::center ? SS_CENTER : params.alignh == aligns::left ? SS_LEFT : SS_RIGHT) |
		(params.alignv == aligns::center ? BS_CENTER : params.alignv == aligns::top ? BS_TOP : BS_BOTTOM));

	switch (state)
	{
	case enabled:
		hBrush = CreateSolidBrush(vRGB(params.bkCol));
		hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdDefCol));
		SetBkColor(mDC, vRGB(params.bkCol));
		SetTextColor(mDC, vRGB(params.textCol));
		break;
	case hovered:
		hBrush = CreateSolidBrush(vRGB(params.bkHovCol));
		hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdHovCol));
		SetBkColor(mDC, vRGB(params.bkHovCol));
		SetTextColor(mDC, vRGB(params.textHovCol));
		break;
	case pressed:
		hBrush = CreateSolidBrush(vRGB(params.bkPreCol));
		hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdPreCol));
		SetBkColor(mDC, vRGB(params.bkPreCol));
		SetTextColor(mDC, vRGB(params.textPreCol));
		break;
	case invalid:
		hBrush = CreateSolidBrush(vRGB(params.bkCol));
		hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdInvCol));
		SetBkColor(mDC, vRGB(params.bkCol));
		SetTextColor(mDC, vRGB(params.textCol));
		break;
	default:
		hBrush = CreateSolidBrush(vRGB(params.bkDisCol));
		hPen = CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdDisCol));
		SetBkColor(mDC, vRGB(params.bkDisCol));
		SetTextColor(mDC, vRGB(params.textDisCol));
		break;
	}

	DeleteObject(SelectObject(mDC, hBrush));
	DeleteObject(SelectObject(mDC, hPen));

	RoundRect(mDC, 2, 2, transform.size.x - 2, transform.size.y - 2, 13, 13);

	BitBlt(hDC, 0, 0, transform.size.x, transform.size.y, mDC, 0, 0, SRCCOPY);
	InvalidateRect(placeholder, NULL, FALSE);

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

void Button::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, REDRAW_IDT, REDRAW_RATE, NULL);
}
LRESULT Button::PlaceholderProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_NCHITTEST:
			return HTTRANSPARENT;
		default:
			return CallWindowProc(defaultStaticProc_LIBVAR, hWnd, uMsg, wParam, lParam);
	}
}
LRESULT Button::ButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BUTTON* obj = objmap[hWnd];

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
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		COLORREF col = vRGB((obj->state == enabled ? obj->params.bkCol : obj->state == hovered ? obj->params.bkHovCol : obj->state == pressed ? obj->params.bkPreCol : obj->params.bkDisCol));
		DeleteObject(obj->hPlaceholderBKBrush);
		obj->hPlaceholderBKBrush = CreateSolidBrush(col);
		SetTextColor(hdcStatic, vRGB(obj->params.textCol));
		SetBkColor(hdcStatic, col);
		return (INT_PTR)obj->hPlaceholderBKBrush;
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

			if (obj->state != pressed && obj->state != disabled && obj->state != invalid) obj->state = hovered;
		}
		break;
	}
	case WM_MOUSELEAVE:
	{
		if (obj)
		{
			if (obj->state != pressed && obj->state != disabled && obj->state != invalid) obj->state = enabled;
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetCapture(hWnd);
		isLMBPressed_LIBVAR = TRUE;
		if (obj)
		{
			if (obj->state != invalid && obj->state != disabled)
				obj->Press();
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		ReleaseCapture();
		isLMBPressed_LIBVAR = FALSE;
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
