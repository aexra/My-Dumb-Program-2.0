#include "Static.h"

map<HWND, Static*> Static::objmap = { };
extern BOOL isLMBPressed_LIBVAR;
extern WNDPROC defaultStaticProc_LIBVAR;
extern ThemeManager* tmr;

Static::Static(HWND _hParWnd, string _Text, V3 _Position, UINT_PTR _nIDEvent, V3 _Size, STATICPARAMS _Params)
{
	text = _Text;
	params = _Params;
	params.charHeight = _Size.z;
	transform.position = _Position;
	nIDEvent = _nIDEvent;
	transform.size = _Size;
	state = enabled;
	laststate = invalid;
	GenWnd(_hParWnd);
	objmap[wnd] = this;
}

void Static::GenWnd(HWND _hParWnd)
{
	wnd = CreateWindow(STATIC_WC, L"name", WS_CHILD | WS_VISIBLE,
		transform.position.x, transform.position.y, transform.size.x, transform.size.y, _hParWnd, NULL, NULL, NULL);
	placeholder = CreateWindowA("static", text.c_str(), WS_CHILD | WS_VISIBLE |
		(params.alignh == aligns::center ? SS_CENTER : params.alignh == aligns::left ? SS_LEFT : SS_RIGHT) |
		(params.alignv == aligns::center ? BS_CENTER : params.alignv == aligns::top ? BS_TOP : BS_BOTTOM),
		7, 7, transform.size.x - 14, transform.size.y - 14, wnd, NULL, NULL, NULL);
	defaultStaticProc_LIBVAR = (WNDPROC)GetWindowLongPtrA(placeholder, GWLP_WNDPROC);
	SetWindowLongPtrA(placeholder, GWLP_WNDPROC, (LONG_PTR)PlaceholderProc);
	HFONT hf = CreateFont(params.charHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, params.font.c_str());
	SendMessage(placeholder, WM_SETFONT, (WPARAM)hf, 0);
}

void Static::UpdateAligning()
{
	SetWindowLongA(placeholder, GWL_STYLE, WS_CHILD | WS_VISIBLE |
		(params.alignh == aligns::center ? SS_CENTER : params.alignh == aligns::left ? SS_LEFT : SS_RIGHT) |
		(params.alignv == aligns::center ? BS_CENTER : params.alignv == aligns::top ? BS_TOP : BS_BOTTOM));
	InvalidateRect(placeholder, NULL, TRUE);
}

BOOL Static::MoveControl(V3 _To)
{
	transform.Translate(_To);
	MoveWindow(wnd, _To.x, _To.y, transform.size.x, transform.size.y, TRUE);
	MoveWindow(placeholder, _To.x+7, _To.y+7, transform.size.x-14, transform.size.y-14, TRUE);
	return 1;
}

string Static::SetText(string _Text)
{
	SetWindowTextA(wnd, _Text.c_str());
	SetWindowTextA(placeholder, _Text.c_str());
	return text = _Text;
}

string Static::AddText(string _Text)
{
	text += _Text;
	SetWindowTextA(wnd, text.c_str());
	SetWindowTextA(placeholder, text.c_str());
	return text;
}

string Static::AddLine(string _Text)
{
	text += "\n" + _Text;
	SetWindowTextA(wnd, text.c_str());
	SetWindowTextA(placeholder, text.c_str());
	return text;
}

void Static::CommandHandler(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

}

void Static::TimerManager(HWND hWnd, WPARAM wParam, LPARAM lParam)
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

void Static::SetAlignV(aligns _Al)
{
	params.alignv = _Al;
	UpdateAligning();
}
void Static::SetAlignH(aligns _Al)
{
	params.alignh = _Al;
	UpdateAligning();
}
HWND Static::GetPlaceholder()
{
	return placeholder;
}
void Static::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, REDRAW_IDT, REDRAW_RATE, NULL);
}
LRESULT Static::PlaceholderProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCHITTEST:
		return HTTRANSPARENT;
	default:
		return CallWindowProc(defaultStaticProc_LIBVAR, hWnd, uMsg, wParam, lParam);
	}
}
LRESULT Static::StaticProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	STATIC* obj = objmap[hWnd];

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (obj) obj->CommandHandler(hWnd, wParam, lParam);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		PALETTE plt = tmr->GetPalette();
		HWND wnd = obj->GetWindow();
		HWND placeholder = obj->GetPlaceholder();
		TRANSFORM& transform = obj->transform;
		states& state = obj->state;
		STATICPARAMS& params = obj->params;

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
		case enabled:
			hBrush = CreateSolidBrush(vRGB(params.bkCol));
			hPen = (params.bdWidth != 0 ? CreatePen(BS_SOLID, params.bdWidth, vRGB(params.bdDefCol))
				: (HPEN)GetStockObject(NULL_PEN));
			SetBkColor(mDC, vRGB(params.bkCol));
			SetTextColor(mDC, vRGB(params.textCol));
			break;
		}

		DeleteObject(SelectObject(mDC, hBrush));
		DeleteObject(SelectObject(mDC, hPen));

		RoundRect(mDC, 2, 2, transform.size.x - 2, transform.size.y - 2, 13, 13);

		BitBlt(hDC, 0, 0, transform.size.x, transform.size.y, mDC, 0, 0, SRCCOPY);
		InvalidateRect(placeholder, NULL, FALSE);

		SelectObject(mDC, oldp);
		SelectObject(mDC, oldb);

		EndPaint(wnd, &ps);
		DeleteDC(mDC);
		DeleteBitmap(mBM);
		DeleteObject(hBrush);
		DeleteObject(hPen);
		DeleteObject(oldp);
		DeleteObject(oldb);
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
		COLORREF col = vRGB((obj->state == enabled ? obj->params.bkCol : V3() /*TODO: disabled ver*/));
		DeleteObject(obj->hPlaceholderBKBrush);
		obj->hPlaceholderBKBrush = CreateSolidBrush(col);
		SetTextColor(hdcStatic, vRGB(obj->params.textCol));
		SetBkColor(hdcStatic, col);
		return (INT_PTR)obj->hPlaceholderBKBrush;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
