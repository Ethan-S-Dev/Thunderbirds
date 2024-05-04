#pragma once
#include "../Extensions.h"
#include "Ship.h"
#include "IScreen.h"
#include "../Core/IPainter.h"
#include "GameObject.h"

constexpr auto ZERO_EXIT_NAME = '0';
constexpr auto NINE_EXIT_NAME = '9';

class Exit : public GameObject {
private:
	const IScreen* _screen;
public:
	Exit(char name, const IScreen* screen) : GameObject(name), _screen(screen) {}
public:
	bool AddPosition(const Point point) {
		if (contains(_positions, point))
			return false;

		if (!PointCanBeAdded(point))
			return false;

		_positions.push_back(point);
		return true;
	}
	bool IsValid() const {
		return Ship::CanFitInPositions(_positions);
	}
	bool CanFitBigShip() const {
		return Ship::CanFitInPositions(_positions, BIG_SHIP_NAME);
	}
	const Point& GetLowestPoint() const {
		auto lowestIndex = 0;
		for (auto i = 1; i < _positions.size(); i++)
		{
			if (_positions[i].Y > _positions[lowestIndex].Y) {
				continue;
			}

			if (_positions[i].Y == _positions[lowestIndex].Y && _positions[i].X > _positions[lowestIndex].X) {
				continue;
			}

			lowestIndex = i;
		}

		return _positions[lowestIndex];
	}
private:
	bool PointCanBeAdded(const Point point) const{
		auto size = _positions.size();
		switch (size)
		{
		case 0:
		{
			return true;
		}
		case 1:
		{
			auto& currentPoint = _positions[0];
			auto xDistence = ((currentPoint.X - point.X) * (currentPoint.X - point.X));
			auto yDistence = ((currentPoint.Y - point.Y) * (currentPoint.Y - point.Y));

			return 0 <= xDistence <= 1 && 0 <= yDistence <= 1;
		}
		case 2:
		{
			auto& currentPointA = _positions[0];
			auto& currentPointB = _positions[1];
			//[ ][A][ ] | [ ][B][ ]
			//[ ][B][ ] | [ ][A][ ]
			if (currentPointA.X == currentPointB.X) {
				if (((currentPointA.X - point.X) * (currentPointA.X - point.X)) != 1)
					return false;
				if (currentPointA.Y != point.Y && currentPointB.Y != point.Y)
					return false;
				return true;
			}
			//[ ][ ] | [ ][ ]
			//[A][B] | [B][A]
			//[ ][ ] | [ ][ ]
			if (currentPointA.Y == currentPointB.Y) {
				if (((currentPointA.Y - point.Y) * (currentPointA.Y - point.Y)) != 1)
					return false;
				if (currentPointA.X != point.X && currentPointB.X != point.X)
					return false;
				return true;
			}
			//[A][ ] | [B][ ] | [ ][A] | [ ][B]
			//[ ][B] | [ ][A] | [B][ ] | [A][ ]
			if (point.X == currentPointA.X && point.Y == currentPointB.Y)
				return true;

			if (point.Y == currentPointA.Y && point.X == currentPointB.X)
				return true;

			return false;
		}
		case 3:
		{
			auto& currentPointA = _positions[0];
			auto& currentPointB = _positions[1];
			auto& currentPointC = _positions[2];

			// [A][ ] | [ ][A] | [A][C] | [C][A] | [B][ ] | [ ][B] | [B][C] | [C][B]
			// [B][C] | [C][B] | [B][ ] | [ ][B] | [A][C] | [C][A] | [A][ ] | [ ][A]
			if (currentPointA.X == currentPointB.X) {
				if (point.X != currentPointC.X)
					return false;

				return point.Y == currentPointA.Y || point.Y == currentPointB.Y;
			}

			// [A][ ] | [ ][A] | [A][B] | [B][A] | [C][ ] | [ ][C] | [C][B] | [B][C]
			// [C][B] | [B][C] | [C][ ] | [ ][C] | [A][B] | [B][A] | [A][ ] | [ ][A]
			if (currentPointA.X == currentPointC.X) {
				if (point.X != currentPointB.X)
					return false;

				return point.Y == currentPointA.Y || point.Y == currentPointC.Y;
			}

			// [B][ ] | [ ][B] | [B][A] | [A][B] | [C][ ] | [ ][C] | [C][A] | [A][C]
			// [C][A] | [A][C] | [C][ ] | [ ][C] | [B][A] | [A][B] | [B][ ] | [ ][B]

			if (point.X != currentPointA.X)
				return false;
			return point.Y == currentPointB.Y || point.Y == currentPointC.Y;
			// 4*3*2 = 24 options
		}
		default:
		{
			return false;
		}
		}
	}
};