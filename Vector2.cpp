#include "Vector2.h"


Vector2::Vector2()
{
	x = y = 0;
	Invalidate();
	Len();
}
Vector2::Vector2(DOUBLE _x, DOUBLE _y)
{
	x = _x;
	y = _y;
	Invalidate();
	Len();
}
DOUBLE Vector2::Len()
{
	if (length == -1) return length = sqrt(x * x + y * y);
	else return length;
}
void Vector2::Invalidate()
{
	length = -1;
}
Vector2 Vector2::Normalized()
{
	return *this / this->Len();
}
DOUBLE Vector2::cos(Vector2 _v2)
{
	return ((x * _v2.x + y * _v2.y) / (Len() * _v2.Len()));
}
//
//		”Õ¿–Õ€≈
//
Vector2 Vector2::operator += (const Vector2& _Other)
{
	x += _Other.x;
	y += _Other.y;
	Invalidate();
	return *this;
}
Vector2 Vector2::operator += (const DOUBLE& _Other)
{
	x += _Other;
	y += _Other;
	Invalidate();
	return *this;
}
Vector2 Vector2::operator -= (const Vector2& _Other)
{
	x -= _Other.x;
	y -= _Other.y;
	Invalidate();
	return *this;
}
Vector2 Vector2::operator -= (const DOUBLE& _Other)
{
	x -= _Other;
	y -= _Other;
	Invalidate();
	return *this;
}
Vector2 Vector2::operator *= (const DOUBLE& _Other)
{
	x *= _Other;
	y *= _Other;
	Invalidate();
	return *this;
}
Vector2 Vector2::operator /= (const DOUBLE& _Other)
{
	x /= _Other;
	y /= _Other;
	Invalidate();
	return *this;
}
Vector2 Vector2::operator = (const Vector2& _Other)
{
	x = _Other.x;
	y = _Other.y;
	Invalidate();
	return *this;
}
//
//		¡»Õ¿–Õ€≈
//
Vector2 operator + (const Vector2& _Left, const Vector2& _Right)
{
	return Vector2(_Left.x + _Right.x, _Left.y + _Right.y);
}
Vector2 operator + (const Vector2& _Left, const DOUBLE& _Right)
{
	return Vector2(_Left.x + _Right, _Left.y + _Right);
}
Vector2 operator - (const Vector2& _Left, const Vector2& _Right)
{
	return Vector2(_Left.x - _Right.x, _Left.y - _Right.y);
}
Vector2 operator - (const Vector2& _Left, const DOUBLE& _Right)
{
	return Vector2(_Left.x - _Right, _Left.y - _Right);
}
DOUBLE operator * (const Vector2& _Left, const Vector2& _Right)
{
	return _Left.x * _Right.x + _Left.y * _Right.y;
}
Vector2 operator * (const Vector2& _Left, const DOUBLE& _Right)
{
	return Vector2(_Left.x * _Right, _Left.y * _Right);
}
Vector2 operator / (const Vector2& _Left, const DOUBLE& _Right)
{
	return Vector2((DOUBLE)_Left.x / _Right, (DOUBLE)_Left.y / _Right);
}
BOOL operator == (const Vector2& _Left, const Vector2& _Right)
{
	return (_Left.x == _Right.x && _Left.y == _Right.y);
}
//
//		Õ¿œ–¿¬À≈Õ»ﬂ
//
const Vector2 Vector2::ZERO = Vector2(0, 0);
const Vector2 Vector2::RIGHT = Vector2(1, 0);
const Vector2 Vector2::LEFT = Vector2(-1, 0);
const Vector2 Vector2::UP = Vector2(0, 1);
const Vector2 Vector2::DOWN = Vector2(0, -1);