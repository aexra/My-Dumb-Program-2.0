#include "Vector3.h"


Vector3::Vector3()
{
	x = y = z = 0;
	Invalidate();
	Len();
}
Vector3::Vector3(long _x, long _y, long _z)
{
	x = _x;
	y = _y;
	z = _z;
	Invalidate();
	Len();
}
DOUBLE Vector3::Len()
{
	if (length == -1) return length = sqrt(x * x + y * y + z * z);
	else return length;
}
void Vector3::Invalidate()
{
	length = -1;
}
Vector3 Vector3::Normalized()
{
	return *this / this->Len();
}
DOUBLE Vector3::cos(Vector3 _v3)
{
	return ((x * _v3.x + y * _v3.y + z * _v3.z) / (Len() * _v3.Len()));
}
//
//		”Õ¿–Õ€≈
//
Vector3 Vector3::operator += (const Vector3& _Other)
{
	x += _Other.x;
	y += _Other.y;
	z += _Other.z;
	Invalidate();
	return *this;
}
Vector3 Vector3::operator += (const DOUBLE& _Other)
{
	x += _Other;
	y += _Other;
	z += _Other;
	Invalidate();
	return *this;
}
Vector3 Vector3::operator -= (const Vector3& _Other)
{
	x -= _Other.x;
	y -= _Other.y;
	z -= _Other.z;
	Invalidate();
	return *this;
}
Vector3 Vector3::operator -= (const DOUBLE& _Other)
{
	x -= _Other;
	y -= _Other;
	z -= _Other;
	Invalidate();
	return *this;
}
Vector3 Vector3::operator *= (const DOUBLE& _Other)
{
	x *= _Other;
	y *= _Other;
	z *= _Other;
	Invalidate();
	return *this;
}
Vector3 Vector3::operator /= (const DOUBLE& _Other)
{
	x /= _Other;
	y /= _Other;
	z /= _Other;
	Invalidate();
	return *this;
}
Vector3 Vector3::operator = (const Vector3& _Other)
{
	x = _Other.x;
	y = _Other.y;
	z = _Other.z;
	Invalidate();
	return *this;
}
//
//		¡»Õ¿–Õ€≈
//
Vector3 operator + (const Vector3& _Left, const Vector3& _Right)
{
	return Vector3(_Left.x + _Right.x, _Left.y + _Right.y, _Left.z + _Right.z);
}
Vector3 operator + (const Vector3& _Left, const DOUBLE& _Right)
{
	return Vector3(_Left.x + _Right, _Left.y + _Right, _Left.z + _Right);
}
Vector3 operator - (const Vector3& _Left, const Vector3& _Right)
{
	return Vector3(_Left.x - _Right.x, _Left.y - _Right.y, _Left.z - _Right.z);
}
Vector3 operator - (const Vector3& _Left, const DOUBLE& _Right)
{
	return Vector3(_Left.x - _Right, _Left.y - _Right, _Left.z - _Right);
}
DOUBLE operator * (const Vector3& _Left, const Vector3& _Right)
{
	return _Left.x * _Right.x + _Left.y * _Right.y + _Left.z * _Right.z;
}
Vector3 operator * (const Vector3& _Left, const DOUBLE& _Right)
{
	return Vector3(_Left.x * _Right, _Left.y * _Right, _Left.z * _Right);
}
Vector3 operator / (const Vector3& _Left, const DOUBLE& _Right)
{
	return Vector3((DOUBLE)_Left.x / _Right, (DOUBLE)_Left.y / _Right, (DOUBLE)_Left.z / _Right);
}
BOOL operator == (const Vector3& _Left, const Vector3& _Right)
{
	return (_Left.x == _Right.x && _Left.y == _Right.y && _Left.z == _Right.z);
}
//
//		Õ¿œ–¿¬À≈Õ»ﬂ
//
const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::RIGHT = Vector3(1, 0, 0);
const Vector3 Vector3::LEFT = Vector3(-1, 0, 0);
const Vector3 Vector3::FORTH = Vector3(0, 1, 0);
const Vector3 Vector3::BACK = Vector3(0, -1, 0);
const Vector3 Vector3::UP = Vector3(0, 0, 1);
const Vector3 Vector3::DOWN = Vector3(0, 0, -1);
