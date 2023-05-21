#include "Transform.h"

V3 Transform::TranslateBy(V3 _By)
{
	position.x += _By.x;
	position.y += _By.y;
	position.z += _By.z;

	return position;
}

V3 Transform::Translate(V3 _To)
{
	position.x = _To.x;
	position.y = _To.y;
	position.z = _To.z;

	return position;
}

V3 Transform::Resize(V3 _By)
{
	position.x = _By.x;
	position.y = _By.y;
	position.z = _By.z;

	return size;
}