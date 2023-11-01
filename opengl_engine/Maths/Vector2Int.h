#pragma once
#include <string>
#include <functional>

enum FloatToIntType;

struct Vector2Int
{
	int x{ 0 };
	int y{ 0 };

	static const Vector2Int zero;
	static const Vector2Int unitX;
	static const Vector2Int unitY;

	Vector2Int() = default;
	Vector2Int(int xP, int yP) : x{ xP }, y{ yP } {}
	Vector2Int(const class Vector2 vec2, FloatToIntType floatToIntType);

	void set(float xP, float yP);
	float lengthSq() const;
	float length() const;
	

	friend Vector2Int operator+(const Vector2Int& left, const Vector2Int& right)
	{
		return { left.x + right.x, left.y + right.y };
	}

	friend Vector2Int operator-(const Vector2Int& left, const Vector2Int& right)
	{
		return { left.x - right.x, left.y - right.y };
	}

	friend Vector2Int operator*(const Vector2Int& vec, int scalar)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	friend Vector2Int operator*(int scalar, const Vector2Int& vec)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	Vector2Int& operator+=(const Vector2Int& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Vector2Int& operator-=(const Vector2Int& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

	Vector2Int& operator*=(const int scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2Int operator-() const
	{
		return Vector2Int(-x, -y);
	}

	bool operator==(Vector2Int& right) const
	{
		return (x == right.x && y == right.y);
	}

	bool operator!=(Vector2Int& right) const
	{
		return (x != right.x || y != right.y);
	}

	std::string toString() const
	{
		return std::to_string(x) + "  " + std::to_string(y) + " \n";
	}
};

struct Vector2IntHash
{
	std::size_t operator()(const Vector2Int& vec) const
	{
		std::size_t h1 = std::hash<int>{}(vec.x);
		std::size_t h2 = std::hash<int>{}(vec.y);
		return h1 ^ (h2 << 1);
	}
};

struct Vector2IntEqual
{
	bool operator()(const Vector2Int& vec1, const Vector2Int& vec2) const
	{
		return vec1.x == vec2.x && vec1.y == vec2.y;
	}
};



enum FloatToIntType
{
	Round,
	Floor,
	Ceil
};