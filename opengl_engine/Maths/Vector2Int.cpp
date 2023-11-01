#include "Vector2Int.h"
#include "Maths.h"
#include "Vector2.h"

const Vector2Int Vector2Int::zero{ 0, 0 };
const Vector2Int Vector2Int::unitX{ 1, 0 };
const Vector2Int Vector2Int::unitY{ 0, 1 };

Vector2Int::Vector2Int(const Vector2 vec2, FloatToIntType floatToIntType)
{
	switch (floatToIntType)
	{
		case Round:
			x = Maths::round(vec2.x);
			y = Maths::round(vec2.y);
		break;

		case Floor:
			x = Maths::floor(vec2.x);
			y = Maths::floor(vec2.y);
		break;

		case Ceil:
			x = Maths::ceil(vec2.x);
			y = Maths::ceil(vec2.y);
		break;
	}
}

void Vector2Int::set(float xP, float yP)
{
	x = xP;
	y = yP;
}

float Vector2Int::lengthSq() const
{
	return x * x + y * y;
}

float Vector2Int::length() const
{
	return Maths::sqrt(lengthSq());
}