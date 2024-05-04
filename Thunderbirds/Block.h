#pragma once
#include "Extensions.h"
#include "IScreen.h"
#include "IPainter.h"

constexpr auto A_BLOCK_NAME = 'a';
constexpr auto Z_BLOCK_NAME = 'z';

class Block : public GameObject {
private:
	float _tickCounter;
	const IScreen* _screen;
public:
	Block(char name, const IScreen* screen) 
		: GameObject(name) ,
		_tickCounter(0), _screen(screen) {
	}
public:
	bool AddPosition(const Point point) {
		if (contains(_positions, point))
			return false;

		_positions.push_back(point);
		return true;
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
	
public:
	std::vector<GameObject*> CollidedEntities(Direction direction) const override {
		return FindEntitiesInTheWay(direction);
	}
	void PhysicsUpdate(float elapsedTime, const IController& controller) override {
		_tickCounter += elapsedTime;
		if (_tickCounter < 1)
			return;
		_tickCounter -= 1;
		int noCapacity = _positions.size();
		if (CanBeMoved(Direction::Down, noCapacity).CanBeMoved) {
			Move(Direction::Down);
		}
	}
	MoveResult CanBeMoved(Direction direction, int& carryCapacity) const override {
		carryCapacity -= (int)_positions.size();
		if (carryCapacity < 0) {
			MoveResult result{ 0 };

			// try to carry the blocks upwords with now carry capacity should make the ship die
			if (direction.nY > 0) {
				result.CanBeMoved = false;
				result.FailReason = MoveFailReason::InsufficientCarryCapacity;
				return result;
			}

			// try to push blocks to wall should not make the ship die
			result = InnerCanBeMoved(direction, carryCapacity);
			if (result.CanBeMoved) {
				result.CanBeMoved = false;
				result.FailReason = MoveFailReason::InsufficientCarryCapacity;
			}
			return result;
		}

		return InnerCanBeMoved(direction, carryCapacity);
	}
	void Move(Direction direction) override {
		if (direction.nX == 0 && direction.nY == 0) {
			return;
		}

		auto entitiesInTheWay = FindEntitiesInTheWay(direction);
		for (auto& entity : entitiesInTheWay)
		{
			entity->Move(direction);
		}

		for (auto& point : _positions)
		{
			point.X += direction.nX;
			point.Y += direction.nY;
			auto newPoint = _screen->ClipToScreen(point);
			point.X = newPoint.X;
			point.Y = newPoint.Y;
		}
	}
	int CalculateBlocksCarryedBy(std::vector<char>& CarriedBlocksNames) const override {
		if (contains(CarriedBlocksNames, Name())) {
			return 0;
		}

		CarriedBlocksNames.push_back(Name());
		auto ret = _positions.size();
		for (auto entity : _screen->Entities()) {
			if (entity->IsEqual(*this)) {
				continue;
			}

			if (!HitObjectByMoving(Direction::Up, *entity)) {
				continue;
			}

			auto infinity = INT_MAX;
			auto result = entity->CanBeMoved(Direction::Down, infinity);
			if (result.FailReason == MoveFailReason::HitWall) {
				continue;
			}

			if (result.FailReason == MoveFailReason::HitShip) {
				continue;
			}

			ret += entity->CalculateBlocksCarryedBy(CarriedBlocksNames);
		}

		return ret;
	}
private:
	bool HitEntityByMoving(Direction direction) const {
		for (auto entity : _screen->Entities()) {
			if (entity->IsEqual(*this)) {
				continue;
			}

			for (auto point : _positions)
			{
				point.X += direction.nX;
				point.Y += direction.nY;
				point = _screen->ClipToScreen(point);
				if (entity->Collides(point)) {
					return true;
				}	
			}
		}
		return false;
	}
	bool HitWallByMoving(Direction direction) const {
		for (auto point : _positions)
		{
			point.X += direction.nX;
			point.Y += direction.nY;
			point = _screen->ClipToScreen(point);
			if (_screen->IsWall(point)) {
				return true;
			}		
		}
		return false;
	}
	std::vector<GameObject*> FindEntitiesInTheWay(Direction direction) const {
		std::vector<GameObject*> entitiesInTheWay;
		for (auto entity : _screen->Entities()) {
			if (entity->IsEqual(*this)) {
				continue;
			}

			if (HitObjectByMoving(direction, *entity)) {
				entitiesInTheWay.push_back(entity);
			}
		}
		return entitiesInTheWay;
	}
	bool HitObjectByMoving(Direction direction, const GameObject& entity) const {
		for (auto point : _positions)
		{
			point.X += direction.nX;
			point.Y += direction.nY;
			point = _screen->ClipToScreen(point);
			if (entity.Collides(point)) {
				return true;
			}
		}
		return false;
	}
	MoveResult CanMoveEntitiesByMoving(Direction direction, int& carryCapacity) const {
		auto entitiesInTheWay = FindEntitiesInTheWay(direction);
		for (const auto entity : entitiesInTheWay)
		{
			auto result = entity->CanBeMoved(direction, carryCapacity);
			if (!result.CanBeMoved) {
				return result;
			}

			carryCapacity -= entity->GetPositions().size();
		}
		MoveResult res = { .CanBeMoved = true };
		return res;
	}
	MoveResult InnerCanBeMoved(Direction direction, int& carryCapacity) const {
		MoveResult res = { 0 };
		if (direction.nX == 0 && direction.nY == 0) {
			res.CanBeMoved = true;
			return res;
		}

		if (HitWallByMoving(direction)) {
			res.CanBeMoved = false;
			res.FailReason = MoveFailReason::HitWall;
			return res;
		}

		return CanMoveEntitiesByMoving(direction, carryCapacity);
	}
};