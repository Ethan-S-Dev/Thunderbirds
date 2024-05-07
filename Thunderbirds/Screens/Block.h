#pragma once
#include "IScreen.h"

constexpr auto A_BLOCK_NAME = 'a';
constexpr auto Z_BLOCK_NAME = 'z';

class Block : public GameObject {
private:
	float _tickCounter;
	const IScreen* _screen;
public:
	Block(char name, const IScreen* screen);
public:
	bool AddPosition(const Point point);
	const Point& GetLowestPoint() const;
public:
	std::vector<GameObject*> FindEntitiesInTheWay(Direction direction) const override;
	void PhysicsUpdate(float elapsedTime, const IController& controller) override;
	MoveResult CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesInTheWay) const override;
	void Move(Direction direction, std::vector<GameObject*>& allReadyMoved) override;
	MoveResult CanMoveBy(Direction direction) const override;
	int CalculateBlocksCarryedByEntity(std::vector<char>& CarriedBlocksNames) const override;
private:
	bool HitEntityByMoving(Direction direction) const;
	bool HitWallByMoving(Direction direction) const;
	bool HitObjectByMoving(Direction direction, const GameObject& entity) const;
	void FallDownBy(int fallRate);
};