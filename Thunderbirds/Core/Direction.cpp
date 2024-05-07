#include "Direction.h"

const Direction Direction::Up = { 0,1 };
const Direction Direction::Down = { 0,-1 };
const Direction Direction::Left = { -1,0 };
const Direction Direction::Right = { 1,0 };

Direction::Direction(int nX, int nY) : 
	nX(nX), 
	nY(nY) 
{}

Direction::Direction() : 
	Direction(0, 0) 
{}