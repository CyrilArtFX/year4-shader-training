#pragma once
#include <Maths/Vector3.h>

struct Color
{
	int r{ 0 };
	int g{ 0 };
	int b{ 0 };
	int a{ 0 };

	static const Color black;
	static const Color white;
	static const Color grey;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color magenta;
	static const Color yellow;
	static const Color cyan;
	static const Color transparency;

	Color() = default;
	Color(int rP, int gP, int bP, int aP) : r{ rP }, g{ gP }, b{ bP }, a{ aP } {}

	void set(int rP, int gP, int bP, int aP);

	Vector3 toVector();

	/*
	* Create a color from hue saturation and values.
	* h - 0/360
	* s - 0/100
	* v - 0/100
	*/
	static Color HSV(float h, float s, float v);
};

