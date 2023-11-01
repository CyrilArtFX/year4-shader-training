#pragma once
#include <string>

struct Vector2
{
	float x { 0.0f };
	float y { 0.0f };

	static const Vector2 zero;
	static const Vector2 unitX;
	static const Vector2 unitY;

	Vector2() = default;
	Vector2(float xP, float yP) : x{ xP }, y{ yP } {}
	Vector2(const class Vector3 vec3);
	Vector2(const class Vector2Int vec2Int);

	void set(float xP, float yP);
	float lengthSq() const;
	float length() const;
	void normalize();
	void clamp(const Vector2& min, const Vector2& max);
	void clampMagnitude(float magnitude);


	static Vector2 normalize(const Vector2& vec)
	{
		Vector2 temp = vec;
		temp.normalize();
		return temp;
	}

	static float dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	static Vector2 lerp(const Vector2& a, const Vector2& b, float f)
	{
		return Vector2(a + f * (b - a));
	}


	friend Vector2 operator+(const Vector2& left, const Vector2& right)
	{
		return { left.x + right.x, left.y + right.y };
	}

	friend Vector2 operator-(const Vector2& left, const Vector2& right)
	{
		return { left.x - right.x, left.y - right.y };
	}

	friend Vector2 operator*(const Vector2& vec, float scalar)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	friend Vector2 operator*(float scalar, const Vector2& vec)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	Vector2& operator+=(const Vector2& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

	Vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	std::string toString() const
	{
		return std::to_string(x) + "  " + std::to_string(y) + " \n";
	}
};
