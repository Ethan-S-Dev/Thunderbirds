#include "Ship.h"

bool Ship::CanFitInPositions(const std::vector<Point>& positions, const char name) {
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

bool Ship::CanFitInPositions(const std::vector<Point>& positions) {
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

Ship::Ship(char name, const IScreen* screen) : 
	GameObject(name), 
	_isActive(false), 
	_screen(screen), 
	_direction(0, 0), 
	_secCounter(0.f), 
	_isDead(false), 
	_carryCapacity(name == BIG_SHIP_NAME ? 6 : 2)
{}

void Ship::Activete() {
	_isActive = true;
	_direction = { 0,0 };
}

void Ship::Deactivete() {
	_isActive = false;
	_direction = { 0,0 };
}

bool Ship::AddPosition(const Point point) {
	if (contains(_positions, point))
		return false;

	if (!PointCanBeAdded(point))
		return false;

	_positions.push_back(point);
	return true;
}

bool Ship::IsValid() const {
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

const Point& Ship::GetLowestPoint() const {
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

const Point& Ship::GetHighestPoint() const {
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

void Ship::Stop() {
	_direction = { 0,0 };
}

bool Ship::IsDead() const {
	return _isDead;
}

void Ship::Update(float elapsedTime, const IController& controller) {
	if (!_isActive) {
		return;
	}

	UpdateDirectionFromController(controller);
}

void Ship::PhysicsUpdate(float elapsedTime, const IController& controller) {
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

MoveResult Ship::CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesNeedToBeMoved) const {
	// prevent circular move checks
	if (contains(entitiesNeedToBeMoved, (GameObject*)this)) {
		MoveResult res = { .CanBeMoved = true, .MoveCapacityCost = 0 };
		return res;
	}

	MoveResult res{ false, MoveFailReason::HitShip };
	return res;
}

MoveResult Ship::CanMoveBy(Direction direction) const {
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

void Ship::Move(Direction direction, std::vector<GameObject*>& allReadyMoved) {
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

std::vector<GameObject*> Ship::FindEntitiesInTheWay(Direction direction) const {
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