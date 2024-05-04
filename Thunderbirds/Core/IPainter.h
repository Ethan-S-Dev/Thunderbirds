#pragma once
#include "Point.h"

class IPainter {
public:
	virtual void Draw(const Point& point, const char c) = 0;
};