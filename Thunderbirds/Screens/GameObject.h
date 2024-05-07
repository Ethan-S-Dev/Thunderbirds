#pragma once

#include <vector>

#include "MoveResult.h"
#include "../Core/Direction.h"
#include "../Core/Point.h"
#include "../Core/IController.h"
#include "../Core/IPainter.h"

class GameObject {
private:
	char _name;
protected:
	std::vector<Point> _positions;
public:
	GameObject(char name);
public:
	virtual void Update(float elapsedTime, const IController& controller);
	virtual void PhysicsUpdate(float elapsedTime, const IController& controller);
	virtual int CalculateBlocksCarryedByEntity(std::vector<char>& carriedBlocksNames) const;
	virtual MoveResult CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesInTheWay) const;
	virtual MoveResult CanMoveBy(Direction direction) const;
	virtual void Move(Direction direction, std::vector<GameObject*>& allReadyMoved);
	virtual std::vector<GameObject*> FindEntitiesInTheWay(Direction direction) const;
public:
	bool Collides(const Point& point) const;
	char Name() const;
	bool Is(const char name) const;
	std::vector<Point>& GetPositions();
	const std::vector<Point>& GetPositions() const;
	bool IsEqual(const GameObject& other) const;
	void Draw(IPainter& painter) const;
};