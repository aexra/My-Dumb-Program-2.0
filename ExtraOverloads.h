#pragma once

#include "Header.h"

//
//					POINT OVERLOADS
//
BOOL operator == (_In_ const POINT& _ptl, _In_ const POINT& _ptr);
POINT operator + (_In_ const POINT& _ptl, _In_ const POINT& _ptr);
POINT operator + (_In_ const POINT& _ptl, _In_ const double& _val);
POINT operator - (_In_ const POINT& _ptl, _In_ const POINT& _ptr);