#pragma once
#include "resources.h"
class Vector2
{
private:
	DOUBLE length;
public:
	DOUBLE x, y;

	Vector2();
	Vector2(DOUBLE _x, DOUBLE _y);
	DOUBLE Len();
	Vector2 Normalized();
	DOUBLE cos(Vector2 _v2);
	void Invalidate();

	Vector2 operator += (const Vector2& _Other);
	Vector2 operator += (const DOUBLE& _Other);
	Vector2 operator -= (const Vector2& _Other);
	Vector2 operator -= (const DOUBLE& _Other);
	Vector2 operator *= (const DOUBLE& _Other);
	Vector2 operator /= (const DOUBLE& _Other);
	Vector2 operator = (const Vector2& _Other);

	friend Vector2 operator + (const Vector2& _Left, const Vector2& _Right);
	friend Vector2 operator + (const Vector2& _Left, const DOUBLE& _Right);
	friend Vector2 operator - (const Vector2& _Left, const Vector2& _Right);
	friend Vector2 operator - (const Vector2& _Left, const DOUBLE& _Right);
	friend DOUBLE operator * (const Vector2& _Left, const Vector2& _Right);
	friend Vector2 operator * (const Vector2& _Left, const DOUBLE& _Right);
	friend Vector2 operator / (const Vector2& _Left, const DOUBLE& _Right);
	friend BOOL operator == (const Vector2& _Left, const Vector2& _Right);

	static const Vector2 ZERO;
	static const Vector2 RIGHT;
	static const Vector2 LEFT;
	static const Vector2 UP;
	static const Vector2 DOWN;
};

typedef Vector2 V2;
typedef Vector2 Vec2;
typedef Vector2 VEC2;
typedef Vector2 VECTOR2;

