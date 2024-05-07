#pragma once
#include "../Extensions.h"
#include "../Core/Direction.h"
#include "../Core/Point.h"
#include "../Core/IController.h"
#include "../Core/IPainter.h"
#include "MoveResult.h"

class GameObject {
private:
	char _name;
protected:
	std::vector<Point> _positions;
public:
	GameObject(char name) : _name(name), _positions() {}
public:
	virtual void Update(float elapsedTime, const IController & controller) {

	}
	virtual void PhysicsUpdate(float elapsedTime, const IController& controller) {

	}
	virtual int CalculateBlocksCarryedByEntity(std::vector<char>& carriedBlocksNames) const {
		return 0;
	}
	virtual MoveResult CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesInTheWay) const {
		return { false };
	}
	virtual MoveResult CanMoveBy(Direction direction) const {
		return { false };
	}
	virtual void Move(Direction direction, std::vector<GameObject*>& allReadyMoved) {
	}
	virtual std::vector<GameObject*> FindEntitiesInTheWay(Direction direction) const {
		return {};
	}
public:
	bool Collides(const Point& point) const {
		const auto& positions = GetPositions();
		return contains(positions, point);
	}
	char Name() const {
		return _name;
	}
	bool Is(const char name) const {
		return _name == name;
	}
	std::vector<Point>& GetPositions() {
		return _positions;
	}
	const std::vector<Point>& GetPositions() const {
		return _positions;
	}
	bool IsEqual(const GameObject& other) const {
		return this == &other;
	}
	void Draw(IPainter& painter) const {
		for (const auto& point : _positions)
		{
			painter.Draw(point, _name);
		}
	}
};