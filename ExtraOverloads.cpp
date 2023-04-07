#include "ExtraOverloads.h"

//
//					POINT OVERLOADS
//
BOOL operator == (const POINT& _ptl, const POINT& _ptr)
{
	return (_ptl.x == _ptr.x && _ptl.y == _ptr.y);
}
BOOL operator != (const POINT& _ptl, const POINT& _ptr)
{
	return !(_ptl.x == _ptr.x && _ptl.y == _ptr.y);
}
POINT operator + (const POINT& _ptl, const POINT& _ptr)
{
	POINT _ptn = { };
	_ptn.x = _ptl.x + _ptr.x;
	_ptn.y = _ptl.y + _ptr.y;
	return _ptn;
}
POINT operator + (const POINT& _ptl, const double& _val)
{
	POINT _ptn = { };
	_ptn.x = _ptl.x + _val;
	_ptn.y = _ptl.y + _val;
	return _ptn;
}
POINT operator - (const POINT& _ptl, const POINT& _ptr)
{
	POINT _ptn = { };
	_ptn.x = _ptl.x - _ptr.x;
	_ptn.y = _ptl.y - _ptr.y;
	return _ptn;
}
POINT operator - (const POINT& _ptl, const double& _val)
{
	POINT _ptn = { };
	_ptn.x = _ptl.x - _val;
	_ptn.y = _ptl.y - _val;
	return _ptn;
}
//
//					RECT OVERLOADS
//
BOOL operator != (const RECT& _rl, const RECT& _rr)
{
	return !(_rl.right == _rr.right && _rl.top == _rr.top && _rl.left == _rr.left && _rl.bottom == _rr.bottom);
}