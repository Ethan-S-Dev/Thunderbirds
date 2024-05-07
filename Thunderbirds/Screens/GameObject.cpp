//#include "GameObject.h"
//
//
//GameObject::GameObject(char name) : 
//	_name(name), 
//	_positions() 
//{}
//
// void GameObject::Update(float elapsedTime, const IController& controller) {}
//
// void GameObject::PhysicsUpdate(float elapsedTime, const IController& controller) {}
//
// int GameObject::CalculateBlocksCarryedByEntity(std::vector<char>& carriedBlocksNames) const {
//	return 0;
//}
//
// MoveResult GameObject::CanBeMoved(Direction direction, std::vector<GameObject*>& entitiesInTheWay) const {
//	return { false };
//}
//
// MoveResult GameObject::CanMoveBy(Direction direction) const {
//	return { false };
//}
//
// void GameObject::Move(Direction direction, std::vector<GameObject*>& allReadyMoved) {}
//
// std::vector<GameObject*> GameObject::FindEntitiesInTheWay(Direction direction) const {
//	return {};
//}
//
// bool GameObject::Collides(const Point& point) const {
//	 const auto& positions = GetPositions();
//	 return contains(positions, point);
// }
//
// char GameObject::Name() const {
//	 return _name;
// }
//
// bool GameObject::Is(const char name) const {
//	 return _name == name;
// }
//
// std::vector<Point>& GameObject::GetPositions() {
//	 return _positions;
// }
//
// const std::vector<Point>& GameObject::GetPositions() const {
//	 return _positions;
// }
//
// bool GameObject::IsEqual(const GameObject& other) const {
//	 return this == &other;
// }
//
// void GameObject::Draw(IPainter& painter) const {
//	 for (const auto& point : _positions)
//	 {
//		 painter.Draw(point, _name);
//	 }
// }