#pragma once


class Direction {
public:
	static const Direction Up;
	static const Direction Down;
	static const Direction Left;
	static const Direction Right;
	int nX;
	int nY;
	Direction(int nX,int nY): nX(nX), nY(nY){}
	Direction() : Direction(0,0) {} 
};

const Direction Direction::Up = { 0,1 };
const Direction Direction::Down = { 0,-1 };
const Direction Direction::Left = { -1,0 };
const Direction Direction::Right = { 1,0 };

