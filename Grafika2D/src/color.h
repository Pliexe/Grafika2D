#include <d2d1.h>

#ifndef COLOR_H
#define COLOR_H

class Color : public D2D1::ColorF
{
public:
	Color(D2D1::ColorF::Enum color) : D2D1::ColorF(color) {}
	Color(float r, float g, float b, float a) : D2D1::ColorF(r, g, b, a) { }
};

#endif