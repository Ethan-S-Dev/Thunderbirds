#pragma once
#include <limits>
#include "../Extensions.h"
#include "IScreen.h"
#include "../Core/IPainter.h"
#include "../Core/IController.h"

constexpr auto BIG_SHIP_NAME = '#';
constexpr auto SMALL_SHIP_NAME = '@';
constexpr auto SPEED = 4.f; // blocks per sec
constexpr auto SHIP_TICK = 1.f / SPEED;

class Ship : public GameObject {
public:
	static bool CanFitInPositions(const std::vector<Point>& positions, const char name);
	static bool CanFitInPositions(const std::vector<Point>& positions);
private:
	bool _isActive;
	const IScreen* _screen;
	Direction _direction;
	float _secCounter;
	bool _isDead;
	int _carryCapacity;
public:
	Ship(char name, const IScreen* screen);
public:
	void Activete();
	void Deactivete();
	bool AddPosition(const Point point);
	bool IsValid() const;
	const Point& GetLowestPoint() const;
	const Point& GetHighestPoint() const;
	void Stop();
	bool IsDead() const;
public:
	void Update(float elapsedTime, const IController& controller) override;
	void PhysicsUpdate(float elapsedTime, const IController& controller) override;
	MoveResult CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesNeedToBeMoved) const override;
	MoveResult CanMoveBy(Direction direction) const override;
	void Move(Direction direction, std::vector<GameObject*>& allReadyMoved) override;
	std::vector<GameObject*> FindEntitiesInTheWay(Direction direction) const override;
private:
	bool PointCanBeAdded(const Point point) const {
		switch (Name())
		{
		case BIG_SHIP_NAME:
			return PointCanBeAddedToBigShip(point);
		case SMALL_SHIP_NAME:
			return PointCanBeAddedToSmallShip(point);
		default:
			return false;
		}
	}
	bool PointCanBeAddedToBigShip(const Point point) const {
		auto size = _positions.size();
		switch (size)
		{
			case 0:
			{
				return true;
			}
			case 1:
			{
				const auto& currentPoint = _positions[0];
				auto xDistence = ((currentPoint.X - point.X) * (currentPoint.X - point.X));
				auto yDistence = ((currentPoint.Y - point.Y) * (currentPoint.Y - point.Y));

				return 0 <= xDistence <= 1 && 0 <= yDistence <= 1;
			}
			case 2:
			{
				const auto& currentPointA = _positions[0];
				const auto& currentPointB = _positions[1];
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
				const auto& currentPointA = _positions[0];
				const auto& currentPointB = _positions[1];
				const auto& currentPointC = _positions[2];

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
	bool PointCanBeAddedToSmallShip(const Point point) const {
		auto size = _positions.size();
		switch (size)
		{
			case 0:
			{
				return true;
			}		
			case 1:
			{
				auto currentPoint = _positions[0];
				return ((currentPoint.X - point.X) * (currentPoint.X - point.X)) == 1;
			}
			default:
			{
				return false;
			}
		}
	}
	void UpdateDirectionFromController(const IController& controller) {
		auto leftButton = controller.GetButtonState(Button::Left);
		auto rightButton = controller.GetButtonState(Button::Right);
		auto upButton = controller.GetButtonState(Button::Up);
		auto downButton = controller.GetButtonState(Button::Down);

		if (leftButton.Pressed || rightButton.Pressed || upButton.Pressed || downButton.Pressed) {
			Direction d;
			d.nX += leftButton.Pressed ? -1 : 0;
			d.nX += rightButton.Pressed ? 1 : 0;
			d.nY += upButton.Pressed ? 1 : 0;
			d.nY += downButton.Pressed ? -1 : 0;

			_direction = d;
		}
	}
	bool HitWallByMoving(Direction direction) const {
		for (auto point : _positions)
		{
			point.X += direction.nX;
			point.Y += direction.nY;
			point = _screen->ClipToScreen(point);
			if (_screen->IsWall(point))
				return true;
		}
		return false;
	}
	
	bool HitObjectByMoving(Direction direction,const GameObject& entity) const {
		for (auto point : _positions)
		{
			point.X += direction.nX;
			point.Y += direction.nY;
			point = _screen->ClipToScreen(point);
			if (entity.Collides(point)){
				return true;
			}
		}
		return false;
	}
	void FindAllEntitiesInTheWay(Direction direction, GameObject& entity, std::vector<GameObject*>& entitiesInTheWay) const {
		if (contains(entitiesInTheWay, &entity)) {
			return;
		}

		entitiesInTheWay.push_back(&entity);
		auto innerEntities = entity.FindEntitiesInTheWay(direction);
		for (auto innerEntity : innerEntities)
		{
			FindAllEntitiesInTheWay(direction, *innerEntity, entitiesInTheWay);
		}
	}
	
	bool ShipCrashedByCarriedBlocks() const {
		auto carryedWeight = CalculateCarriedWeight();
		return carryedWeight > _carryCapacity;
	}
	int CalculateCarriedWeight() const {
		auto ret = 0;
		std::vector<char> blockNames;
		for (auto entity : _screen->Entities())
		{
			if (entity->IsEqual(*this)) {
				continue;
			}
			
			if (!HitObjectByMoving(Direction::Up, *entity)) {
				continue;
			}
			
			std::vector<GameObject*> needToBeMoved;
			needToBeMoved.push_back((GameObject*)this);
			auto result = entity->CanBeMoved(Direction::Down, needToBeMoved);
			if (!result.CanBeMoved && result.FailReason == MoveFailReason::HitWall) {
				continue;
			}

			blockNames.push_back(entity->Name());
			ret += entity->GetPositions().size();
			ret += entity->CalculateBlocksCarryedByEntity(blockNames);
		}
		return ret;
	}

};