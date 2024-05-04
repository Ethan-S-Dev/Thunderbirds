#pragma once
#include "GameObject.h"

class IScreen {
public:
	virtual const std::vector<GameObject*>& Entities() const = 0;
	virtual std::vector<GameObject*>& Entities() = 0;
	virtual bool IsWall(const Point& point) const = 0;
	virtual Point ClipToScreen(const Point& point) const = 0;
};