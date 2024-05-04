#pragma once
#include "../Extensions.h"
#include "IScreen.h"
#include "../Core/IPainter.h"

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
	std::vector<GameObject*> FindEntitiesInTheWay(Direction direction) const override {
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
	void PhysicsUpdate(float elapsedTime, const IController& controller) override {
		_tickCounter += elapsedTime;
		if (_tickCounter < 1)
			return;
		_tickCounter -= 1;

		if (CanMoveBy(Direction::Down).CanBeMoved) {
			FallDownBy(1);
		}
	}
	MoveResult CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesInTheWay) const override {
		MoveResult result = { .CanBeMoved = true, .MoveCapacityCost = (int)_positions.size() };
		if (contains(entitiesInTheWay, (GameObject*)this)) {
			result.MoveCapacityCost = 0;
			return result;
		}

		entitiesInTheWay.push_back((GameObject*)this);

		if (direction.nX == 0 && direction.nY == 0) {
			return result;
		}

		if (HitWallByMoving(direction)) {
			result.CanBeMoved = false;
			result.FailReason = MoveFailReason::HitWall;
			return result;
		}

		auto entitiesInTheWayOfThis = FindEntitiesInTheWay(direction);
		for (auto entity : entitiesInTheWayOfThis)
		{
			auto res = entity->CanBeMoved(direction, entitiesInTheWay);
			if (!res.CanBeMoved) {
				return res;
			}

			result.MoveCapacityCost += res.MoveCapacityCost;
		}

		return result;
	}
	void Move(Direction direction, std::vector<GameObject*>& allReadyMoved) override {
		if (direction.nX == 0 && direction.nY == 0) {
			return;
		}

		if (contains(allReadyMoved, (GameObject*)this)) {
			return;
		}

		allReadyMoved.push_back((GameObject*)this);

		auto entitiesInTheWay = FindEntitiesInTheWay(direction);
		for (auto& entity : entitiesInTheWay)
		{
			entity->Move(direction, allReadyMoved);
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
	MoveResult CanMoveBy(Direction direction) const override {
		if (direction.nX == 0 && direction.nY == 0) {
			MoveResult success = { .CanBeMoved = true };
			return success;
		}

		if (HitWallByMoving(direction)) {
			MoveResult failed = { .CanBeMoved = false, .FailReason = MoveFailReason::HitWall };
			return failed;
		}
		MoveResult result = { .CanBeMoved = true, .MoveCapacityCost = 0 };
		std::vector<GameObject*> needToBeMoved;
		needToBeMoved.push_back((GameObject*)this);
		auto entitiesInTheWay = FindEntitiesInTheWay(direction);
		for (const auto entity : entitiesInTheWay)
		{
			auto res = entity->CanBeMoved(direction, needToBeMoved);
			if (!res.CanBeMoved) {
				return res;
			}

			result.MoveCapacityCost += res.MoveCapacityCost;
		}

		return result;
	}
	int CalculateBlocksCarryedByEntity(std::vector<char>& CarriedBlocksNames) const override {
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
			std::vector<GameObject*> toBeMoved;
			auto result = entity->CanBeMoved(Direction::Down, toBeMoved);
			if (result.FailReason == MoveFailReason::HitWall) {
				continue;
			}

			if (result.FailReason == MoveFailReason::HitShip) {
				continue;
			}

			ret += entity->CalculateBlocksCarryedByEntity(CarriedBlocksNames);
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
	void FallDownBy(int fallRate) {
		for (auto& point : _positions)
		{
			point.Y -= fallRate;
			auto newPoint = _screen->ClipToScreen(point);
			point.X = newPoint.X;
			point.Y = newPoint.Y;
		}
	}
};