#pragma once
#include "../Core/Point.h"

struct Rect {
	Point LeftDown;
	Point RightUp;
	Rect(Point leftDown, Point rightUp);
	Rect(int ldx, int ldy, int rux, int ruy);
	Rect();
};