#pragma once

struct Direction {
	static const Direction Up;
	static const Direction Down;
	static const Direction Left;
	static const Direction Right;
	int nX;
	int nY;
	Direction(int nX, int nY);
	Direction();
};
