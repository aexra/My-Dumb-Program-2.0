#pragma once

#include "Header.h"

//
//					POINT OVERLOADS
//
BOOL operator == (const POINT& _ptl, const POINT& _ptr);
POINT operator + (const POINT& _ptl, const POINT& _ptr);
POINT operator + (const POINT& _ptl, const double& _val);
POINT operator - (const POINT& _ptl, const POINT& _ptr);
POINT operator - (const POINT& _ptl, const double& _val);