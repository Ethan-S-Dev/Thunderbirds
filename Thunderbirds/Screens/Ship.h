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
	static bool CanFitInPositions(const std::vector<Point>& positions, const char name) {
		switch (name)
		{
		case SMALL_SHIP_NAME: {
			return CanFitInPositions(positions);
		}
		case BIG_SHIP_NAME: {
			return positions.size() == 4;
		}
		default: {
			return false;
		}
		}
	}
	static bool CanFitInPositions(const std::vector<Point>& positions) {
		switch (positions.size())
		{
			case 2: {
				return positions[0].Y == positions[1].Y;
			}
			case 4: {
				return true;
			}
			default: {
				return false;
			}
		}
	}
private:
	bool _isActive;
	const IScreen* _screen;
	Direction _direction;
	float _secCounter;
	bool _isDead;
	int _carryCapacity;
public:
	Ship(char name, const IScreen* screen) : GameObject(name), _isActive(false), _screen(screen), _direction(0,0), _secCounter(0.f), _isDead(false), _carryCapacity(name == BIG_SHIP_NAME ? 6 : 2)
	{}
public:
	void Activete() {
		_isActive = true;
		_direction = { 0,0 };
	}
	void Deactivete() {
		_isActive = false;
		_direction = { 0,0 };
	}
	bool AddPosition(const Point point) {
		if (contains(_positions, point))
			return false;

		if (!PointCanBeAdded(point))
			return false;

		_positions.push_back(point);
		return true;
	}
	bool IsValid() const {
		switch (Name())
		{
			case BIG_SHIP_NAME: {
				return _positions.size() == 4;
			}
			case SMALL_SHIP_NAME: {
				return _positions.size() == 2;
			}
			default: {
					return false;
			}
		}
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
	const Point& GetHighestPoint() const {
		auto lowestIndex = 0;
		for (auto i = 1; i < _positions.size(); i++)
		{
			if (_positions[i].Y < _positions[lowestIndex].Y) {
				continue;
			}

			if (_positions[i].Y == _positions[lowestIndex].Y && _positions[i].X < _positions[lowestIndex].X) {
				continue;
			}

			lowestIndex = i;
		}

		return _positions[lowestIndex];
	}
	void Stop() {
		_direction = {0,0};
	}
	bool IsDead() const {
		return _isDead;
	}
public:
	void Update(float elapsedTime, const IController& controller) override {
		if (!_isActive) {
			return;
		}

		UpdateDirectionFromController(controller);
	}
	void PhysicsUpdate(float elapsedTime, const IController& controller) override {
		if (ShipCrashedByCarriedBlocks()) {
			_isDead = true;
			return;
		}

		if (!_isActive) {
			_secCounter = 0;
			return;
		}

		_secCounter += elapsedTime;
		if (_secCounter < SHIP_TICK) {
			return;
		}
		_secCounter -= SHIP_TICK;

		if (_direction.nX == 0 && _direction.nY == 0) {
			return;
		}

		auto moveResult = CanMoveBy(_direction);
		if (!moveResult.CanBeMoved) {
			_direction = { 0,0 };
			return;
		}

		auto leftCarryCapacity = _carryCapacity;
		if (_direction.nY != 1) {
			leftCarryCapacity -= CalculateCarriedWeight();
		}

		if (moveResult.MoveCapacityCost > leftCarryCapacity) {
			_isDead = true;
		}

		std::vector<GameObject*> moved;
		Move(_direction, moved);
	}
	MoveResult CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesNeedToBeMoved) const override {
		// prevent circular move checks
		if (contains(entitiesNeedToBeMoved, (GameObject*)this)) {
			MoveResult res = {.CanBeMoved = true, .MoveCapacityCost = 0 };
			return res;
		}

		MoveResult res{ false, MoveFailReason::HitShip};
		return res;
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
	void Move(Direction direction, std::vector<GameObject*>& allReadyMoved) override {
		if (direction.nX == 0 && direction.nY == 0) {
			return;
		}

		// prevent circular move
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
			ret += entity->CalculateBlocksCarryedByEntity(blockNames);
		}
		return ret;
	}

};