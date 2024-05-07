#pragma once

struct Point {
	static const Point Zero;
	int X;
	int Y;
	Point(int x, int y);
	Point();
	bool operator==(const Point& other) const;
};