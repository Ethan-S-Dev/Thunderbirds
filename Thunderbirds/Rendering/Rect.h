#pragma once
#include "../Core/Point.h"

class Rect {
public:
	Point LeftDown;
	Point RightUp;

	Rect(Point leftDown, Point rightUp) : LeftDown(leftDown), RightUp(rightUp)
	{}
	Rect(int ldx, int ldy, int rux, int ruy) : LeftDown(ldx, ldy), RightUp(rux, ruy)
	{}
	Rect() : Rect(0, 0, 0, 0) {}
};