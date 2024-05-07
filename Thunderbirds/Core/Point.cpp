#include "Point.h"

const Point Point::Zero = { 0,0 };

Point::Point(int x, int y) : 
	X(x), 
	Y(y) 
{}

Point::Point() : 
	Point(0, 0) 
{}

bool Point::operator==(const Point& other) const {
	return X == other.X && Y == other.Y;
}