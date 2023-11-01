#pragma once
#include "Maths.h"
#include <string>

class Vector3
{

public:
	float x;
	float y;
	float z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

	explicit Vector3(float xP, float yP, float zP)
		:x(xP), y(yP), z(zP) {}

	Vector3(const struct Vector2 vec2);

	void set(float xP, float yP, float zP);
	float lengthSq() const;
	float length() const;
	void normalize();

	const float* getAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&x);
	}

	// Vector addition (a + b)
	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend Vector3 operator+(const Vector3& a, const struct Vector2& b);
	friend Vector3 operator+(const struct Vector2& a, const Vector3& b);


	// Vector subtraction (a - b)
	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	friend Vector3 operator-(const Vector3& a, const struct Vector2& b);
	friend Vector3 operator-(const struct Vector2& a, const Vector3& b);


	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	friend Vector3 operator*(const Vector3& left, const struct Vector2& right);
	friend Vector3 operator*(const struct Vector2& left, const Vector3& right);


	// Scalar multiplication
	friend Vector3 operator*(const Vector3& vec, float scalar)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar multiplication
	friend Vector3 operator*(float scalar, const Vector3& vec)
	{
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	// Scalar *=
	Vector3& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	// Vector +=
	Vector3& operator+=(const Vector3& right)
	{
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}
	
	Vector3& operator+=(const struct Vector2& right);


	// Vector -=
	Vector3& operator-=(const Vector3& right)
	{
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	Vector3& operator-=(const struct Vector2& right);

	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	// Normalize the provided vector
	static Vector3 normalize(const Vector3& vec)
	{
		Vector3 temp = vec;
		temp.normalize();
		return temp;
	}

	// Dot product between two vectors (a dot b)
	static float dot(const Vector3& a, const Vector3& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	// Cross product between two vectors (a cross b)
	static Vector3 cross(const Vector3& a, const Vector3& b)
	{
		Vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	static Vector3 smoothDamp(const Vector3& from, const Vector3& to, Vector3& velocity, const float smoothTime, const float deltaTime)
	{
		return Vector3
		{
			Maths::smoothDamp(from.x, to.x, velocity.x, smoothTime, deltaTime),
			Maths::smoothDamp(from.y, to.y, velocity.y, smoothTime, deltaTime),
			Maths::smoothDamp(from.z, to.z, velocity.z, smoothTime, deltaTime)
		};
	}

	// Lerp from A to B by f
	static Vector3 lerp(const Vector3& a, const Vector3& b, float f)
	{
		return Vector3(a + f * (b - a));
	}

	// Reflect V about (normalized) N
	static Vector3 reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2.0f * Vector3::dot(v, n) * n;
	}

	static Vector3 transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

	// This will transform the vector and renormalize the w component
	static Vector3 transformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

	// Transform a Vector3 by a quaternion
	static Vector3 transform(const Vector3& v, const class Quaternion& q);

	std::string toString() const
	{
		return std::to_string(x) + "  " + std::to_string(y) + "  " + std::to_string(z) + " \n";
	}

	static const Vector3 zero;
	static const Vector3 one;
	static const Vector3 unitX;
	static const Vector3 unitY;
	static const Vector3 unitZ;
	static const Vector3 negUnitX;
	static const Vector3 negUnitY;
	static const Vector3 negUnitZ;
	static const Vector3 infinity;
	static const Vector3 negInfinity;
};

