#include "Vector2.h"

Vector2::Vector2()
{
	this->x = 0;
	this->y = 0;
}
Vector2::Vector2(int _x, int _y)
{
	this->x = _x;
	this->y = _y;
}

Vector2 operator + (const Vector2& _Left, const Vector2& _Right)
{
	return Vector2(_Left.x + _Right.x, _Left.y + _Right.y);
}
Vector2 operator - (const Vector2& _Left, const Vector2& _Right)
{
	return Vector2(_Left.x - _Right.x, _Left.y - _Right.y);
}