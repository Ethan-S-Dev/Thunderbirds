#pragma once
#include "Extensions.h"
#include "Direction.h"
#include "Point.h"
#include "IController.h"

enum class MoveFailReason {
	HitWall,
	HitShip,
	InsufficientCarryCapacity,
};

typedef struct {
	bool CanBeMoved;
	MoveFailReason FailReason;
	char CollidedWith;
}MoveResult;

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
	virtual int CalculateBlocksCarryedBy(std::vector<char>& CarriedBlocksNames) const {
		return 0;
	}
	virtual MoveResult CanBeMoved(Direction direction, int& carryCapacity) const {
		return { false };
	}
	virtual void Move(Direction direction) {
	}
	virtual std::vector<GameObject*> CollidedEntities(Direction direction) const {
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