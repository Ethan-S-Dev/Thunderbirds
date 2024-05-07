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
	bool PointCanBeAdded(const Point point) const;
	bool PointCanBeAddedToBigShip(const Point point) const;
	bool PointCanBeAddedToSmallShip(const Point point) const;
	void UpdateDirectionFromController(const IController& controller);
	bool HitWallByMoving(Direction direction) const;
	bool HitObjectByMoving(Direction direction, const GameObject& entity) const;
	void FindAllEntitiesInTheWay(Direction direction, GameObject& entity, std::vector<GameObject*>& entitiesInTheWay) const;
	bool ShipCrashedByCarriedBlocks() const;
	int CalculateCarriedWeight() const;
};