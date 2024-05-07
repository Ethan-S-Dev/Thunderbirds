#pragma once
#include "IScreen.h"
#include "Exit.h"
#include "Ship.h"
#include "Block.h"

enum class ScreenState {
	Lost,
	Won,
	Playing
};

class Screen : public IScreen {
private:
	std::unique_ptr<char[]> _screen;
	int _screenWidth, _screenHight;
	int _timeLimit;
	int _lifePoints;
	std::vector<Block> _blocks;
	std::vector<Ship> _ships;
	std::vector<Exit> _exits;
	std::vector<GameObject*> _entities;
	char _activeShipName;
	float _timePlayed;
	ScreenState _state;
	Point _focusePoint;
	std::string _name;
public:
	Screen(const std::string screenFileName);
public:
	const std::string& Name() const;
	bool IsLoaded() const;
	void Reset();
	void Draw(IPainter& painter) const;
	void PhysicsUpdate(float elapsedTime, const IController& controller);
	void Update(float elapsedTime, const IController& controller);
	int Width() const;
	int Hight() const;
	int StartingLifePoints() const;
	int SecondsLeftToFinishScreen() const;
	ScreenState State() const;
	void GroundShips();
	Point ActiveShipMainPoint();
	char ActiveShipName() const;
public:
	const std::vector<GameObject*>& IScreen::Entities() const;
	std::vector<GameObject*>& IScreen::Entities();
	bool IScreen::IsWall(const Point& point) const;
	Point ClipToScreen(const Point& point) const;
private:
	void LoadScreenCharsFromFile(const std::string screenFileName);
	void CreateBlocksFromScreenChars();
	void CreateShipsFromScreenChars();
	void CreateExitsFromScreenChars();
	bool IsValid() const;
	void ClearScreen();
	void DrawWalls(IPainter& painter) const;
	void DrawExits(IPainter& painter) const;
	void DrawBlocks(IPainter& painter) const;
	void DrawShips(IPainter& painter) const;
	void SortBlocks();
	void SortShips();
	void ReEnterEntitiesInOrder();
	void SwapShipsOnInput(const IController& controller);
	void SwitchToShip(const char name);
	bool ShipInsideExit(Ship& ship, Exit& exit) const;
	void RemoveShipsInExit(Exit& exit);
};