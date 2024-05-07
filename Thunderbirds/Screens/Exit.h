#pragma once
#include "IScreen.h"
#include "GameObject.h"

constexpr auto ZERO_EXIT_NAME = '0';
constexpr auto NINE_EXIT_NAME = '9';

class Exit : public GameObject {
private:
	const IScreen* _screen;
public:
	Exit(char name, const IScreen* screen);
public:
	bool AddPosition(const Point point);
	bool IsValid() const;
	bool CanFitBigShip() const;
	const Point& GetLowestPoint() const;
private:
	bool PointCanBeAdded(const Point point) const;
};