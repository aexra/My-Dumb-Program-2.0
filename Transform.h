#pragma once

#include "Header.h"
#include "Vector3.h"

class Transform
{
public:
	V3 position;
	V3 size;

	V3 Translate(V3 _To);
	V3 TranslateBy(V3 _By);
	V3 Resize(V3 _By);
};

typedef Transform TRANSFORM;