#pragma once


class Point {
public:
	static Point Zero;
	int X;
	int Y;
public:
	Point(int x, int y): X(x), Y(y){

	}
	Point() : Point(0, 0) {

	}
public:
	bool operator==(const Point& other) const {
		return X == other.X && Y == other.Y;
	}
};