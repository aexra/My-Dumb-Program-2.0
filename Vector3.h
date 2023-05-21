#pragma once
#include "Header.h"
class Vector3
{
private:
	LONG length;
public:
	LONG x, y, z;

	Vector3();
	Vector3(long _x, long _y, long _z);
	DOUBLE Len();
	Vector3 Normalized();
	DOUBLE cos(Vector3 _v3);
	void Invalidate();

	Vector3 operator += (const Vector3& _Other);
	Vector3 operator += (const DOUBLE& _Other);
	Vector3 operator -= (const Vector3& _Other);
	Vector3 operator -= (const DOUBLE& _Other);
	Vector3 operator *= (const DOUBLE& _Other);
	Vector3 operator /= (const DOUBLE& _Other);
	Vector3 operator = (const Vector3& _Other);

	friend Vector3 operator + (const Vector3& _Left, const Vector3& _Right);
	friend Vector3 operator + (const Vector3& _Left, const DOUBLE& _Right);
	friend Vector3 operator - (const Vector3& _Left, const Vector3& _Right);
	friend Vector3 operator - (const Vector3& _Left, const DOUBLE& _Right);
	friend DOUBLE operator * (const Vector3& _Left, const Vector3& _Right);
	friend Vector3 operator * (const Vector3& _Left, const DOUBLE& _Right);
	friend Vector3 operator / (const Vector3& _Left, const DOUBLE& _Right);
	friend BOOL operator == (const Vector3& _Left, const Vector3& _Right);

	static const Vector3 ZERO;
	static const Vector3 RIGHT;
	static const Vector3 LEFT;
	static const Vector3 FORTH;
	static const Vector3 BACK;
	static const Vector3 UP;
	static const Vector3 DOWN;
};

typedef Vector3 V3;
typedef Vector3 Vec3;
typedef Vector3 VEC3;
typedef Vector3 VECTOR3;
