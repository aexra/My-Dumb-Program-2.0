#include "ExtraOverloads.h"

//
//					POINT OVERLOADS
//
BOOL operator == (const POINT& _ptl, const POINT& _ptr)
{
	return (_ptl.x == _ptr.x && _ptl.y == _ptr.y);
}
POINT operator + (const POINT& _ptl, const POINT& _ptr)
{
	POINT _ptn = { };
	_ptn.x = _ptl.x + _ptr.x;
	_ptn.y = _ptl.y + _ptr.y;
	return _ptn;
}