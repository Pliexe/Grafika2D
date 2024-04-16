#include <d2d1.h>
#include "point.h"

#pragma once

namespace Grafika
{
    struct Ellipse
    {
        Ellipse(float x, float y, float radiusX, float radiusY) : point(x, y), radiusX(radiusX), radiusY(radiusY) {}
        Ellipse(const PointF &point, float radiusX, float radiusY) : point(point), radiusX(radiusX), radiusY(radiusY) {}

        Ellipse(float x, float y, float radius) : point(x, y), radiusX(radius), radiusY(radius) {}
        Ellipse(const PointF &point, float radius) : point(point), radiusX(radius), radiusY(radius) {}

        Ellipse() = default;
        Ellipse(const Ellipse &) = default;

        PointF point;
        float radiusX;
        float radiusY;

        explicit operator D2D1_ELLIPSE() const { return *reinterpret_cast<const D2D1_ELLIPSE *>(this); }
    };
}