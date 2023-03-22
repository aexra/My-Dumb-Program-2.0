#pragma once

class Vector2
{
public:
	int x, y;

	Vector2();
	Vector2(int _x, int _y);
};

Vector2 operator + (const Vector2& _Left, const Vector2& _Right);
Vector2 operator - (const Vector2& _Left, const Vector2& _Right);

