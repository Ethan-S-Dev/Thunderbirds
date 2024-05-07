#include "Rect.h"

Rect::Rect(Point leftDown, Point rightUp) : 
	LeftDown(leftDown), 
	RightUp(rightUp)
{}

Rect::Rect(int ldx, int ldy, int rux, int ruy) : 
	LeftDown(ldx, ldy), 
	RightUp(rux, ruy)
{}

Rect::Rect() : 
	Rect(0, 0, 0, 0) 
{}