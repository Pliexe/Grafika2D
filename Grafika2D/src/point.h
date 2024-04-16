#include <d2d1.h>

#pragma once

namespace Grafika
{
    struct PointF
    {
        float x;
        float y;

        PointF(float x, float y) : x(x), y(y) {}

        PointF() : x(0), y(0) {}

        PointF operator+(const PointF &p) const { return PointF(x + p.x, y + p.y); }
        PointF operator-(const PointF &p) const { return PointF(x - p.x, y - p.y); }
        PointF operator*(float s) const { return PointF(x * s, y * s); }
        PointF operator/(float s) const { return PointF(x / s, y / s); }

        operator D2D1_POINT_2F() const { return D2D1::Point2F(x, y); }
    };
}