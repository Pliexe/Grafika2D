#include <d2d1.h>

#pragma once

namespace Grafika
{
	template <typename T>
	struct Rect
	{
		T left;
		T top;
		T right;
		T bottom;

		Rect() : left(0), top(0), right(0), bottom(0) {}
		Rect(const Rect &other) : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}
		Rect(T left, T top, T right, T bottom) : left(left), top(top), right(right), bottom(bottom) {}

		explicit operator D2D1_RECT_U() const { return *reinterpret_cast<const D2D1_RECT_U *>(this); }
		explicit operator D2D1_RECT_F() const { return *reinterpret_cast<const D2D1_RECT_F *>(this); }
	};

	using RectF = Rect<float>;
	using RectU = Rect<uint32_t>;

	struct RoundedRect
	{
		RectF rect;
		float radiusX;
		float radiusY;

		RoundedRect() : rect(), radiusX(0), radiusY(0) {}
		RoundedRect(const RectF &rect, float radiusX, float radiusY) : rect(rect), radiusX(radiusX), radiusY(radiusY) {}
		RoundedRect(float left, float top, float right, float bottom, float radiusX, float radiusY) : rect(left, top, right, bottom), radiusX(radiusX), radiusY(radiusY) {}
		
		RoundedRect(const RectF &rect, float radius) : rect(rect), radiusX(radius), radiusY(radius) {}
		RoundedRect(float left, float top, float right, float bottom, float radius) : rect(left, top, right, bottom), radiusX(radius), radiusY(radius) {}

		RoundedRect(const RoundedRect &other) : rect(other.rect), radiusX(other.radiusX), radiusY(other.radiusY) {}


		explicit operator D2D1_ROUNDED_RECT() const { return *reinterpret_cast<const D2D1_ROUNDED_RECT*>(this); }
	};
}