#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "IScreen.h"
#include "Exit.h"
#include "Ship.h"
#include "Block.h"

constexpr auto EMPTY_TILE = ' ';
constexpr auto WALL_TILE = 'W';

constexpr auto MAX_PLAY_TIME = 60 * 60;
constexpr auto MIN_PLAY_TIME = 10;
constexpr auto MIN_SCREEN_WIDTH = 10;
constexpr auto MIN_SCREEN_HIGHT = 5;

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
	Screen(const std::string screenFileName) :
		_screen(nullptr), 
		_screenWidth(0), 
		_screenHight(0), 
		_timeLimit(0),
		_lifePoints(3),
		_blocks(), 
		_ships(), 
		_exits(), 
		_entities(), 
		_activeShipName(BIG_SHIP_NAME),
		_timePlayed(0),
		_state(ScreenState::Playing),
		_focusePoint(0,0),
		_name()
	{
		LoadScreenCharsFromFile(screenFileName);
		CreateBlocksFromScreenChars();
		CreateShipsFromScreenChars();
		CreateExitsFromScreenChars();

		if (IsValid())
		{
			SortBlocks();
			SortShips();
		}
		else {
			ClearScreen();
		}
	}
public:
	const std::string& Name() const {
		return _name;
	}
	bool IsLoaded() const {
		return  _screen != nullptr;
	}
	void Reset() {
		_blocks.clear();
		_ships.clear();
		_exits.clear();
		CreateBlocksFromScreenChars();
		CreateShipsFromScreenChars();
		CreateExitsFromScreenChars();
		_activeShipName = BIG_SHIP_NAME;
		_timePlayed = 0;
		_state = ScreenState::Playing;
	}
	void Draw(IPainter& painter) const {
		DrawWalls(painter);
		DrawExits(painter);
		DrawBlocks(painter);
		DrawShips(painter);
	}
	void PhysicsUpdate(float elapsedTime, const IController& controller) {
		if (_state != ScreenState::Playing) {
			return;
		}

		_timePlayed += elapsedTime;

		for (auto& exit : _exits) {
			exit.PhysicsUpdate(elapsedTime, controller);
		}

		for (auto& ship : _ships) {
			ship.PhysicsUpdate(elapsedTime, controller);
		}

		for (auto& block : _blocks) {
			block.PhysicsUpdate(elapsedTime, controller);
		}

		SortShips();
		SortBlocks();
		ReEnterEntitiesInOrder();

		auto numberOfShipsBeforeRemoving = _ships.size();

		for (auto& exit : _exits) {
			RemoveShipsInExit(exit);
		}

		if (_ships.size() == 0) {
			_state = ScreenState::Won;
			return;
		}

		if (numberOfShipsBeforeRemoving != _ships.size()) {
			_ships.begin()->Activete();
			_activeShipName = _ships.begin()->Name();
		}

		for (auto& ship : _ships)
		{
			if (ship.IsDead()) {
				_state = ScreenState::Lost;
				return;
			}
		}

		if (_timePlayed > _timeLimit) {
			_state = ScreenState::Lost;
		}
	}
	void Update(float elapsedTime, const IController& controller) {
		if (_state != ScreenState::Playing) {
			return;
		}

		SwapShipsOnInput(controller);

		for (auto& exit : _exits) {
			exit.Update(elapsedTime, controller);
		}

		for (auto& block : _blocks) {
			block.Update(elapsedTime, controller);
		}

		for (auto& ship : _ships) {
			ship.Update(elapsedTime, controller);
		}
	}
	int Width() const {
		return _screenWidth;
	}
	int Hight() const {
		return _screenHight;
	}
	int StartingLifePoints() const {
		return _lifePoints;
	}
	int SecondsLeftToFinishScreen() const {
		return _timeLimit - _timePlayed;
	}
	ScreenState State() const {
		return _state;
	}
	void GroundShips() {
		for (auto& ship : _ships) {
			ship.Stop();
		}
	}
	Point ActiveShipMainPoint(){
		auto isActiveShip = [this](const Ship& ship) {
			return ship.Is(_activeShipName);
			};
		auto ship = std::find_if(_ships.begin(), _ships.end(), isActiveShip);

		if (ship != _ships.end()) {
			_focusePoint = ship->GetHighestPoint();
		}
		return _focusePoint;
	}
	char ActiveShipName() const {
		return _activeShipName;
	}
public:
	const std::vector<GameObject*>& IScreen::Entities() const {		
		return _entities;
	}
	std::vector<GameObject*>& IScreen::Entities()  {
		return _entities;
	}
	bool IScreen::IsWall(const Point& point) const {
		auto col = point.X;
		auto row = (_screenHight - 1) - point.Y;
		auto index = (row * _screenWidth) + col;
		return _screen[index] == WALL_TILE;
	}
	Point ClipToScreen(const Point& point) const {
		auto ret = point;
		if (point.X < 0)
		{
			ret.X = _screenWidth - 1;
		}
		else if (point.X >= _screenWidth) {
			ret.X = 0;
		}
		if (point.Y < 0)
		{
			ret.Y = _screenHight - 1;
		}
		else if(point.Y >= _screenHight) {
			ret.Y = 0;
		}
		return ret;
	}
private:
	void LoadScreenCharsFromFile(const std::string screenFileName) {
		std::ifstream screenFile(screenFileName);
		if (!screenFile.is_open()) {
			return;
		}

		char x, T, L, N;
		screenFile >> _screenWidth >> x >> _screenHight >> T >> _timeLimit >> L >> _lifePoints >> N;
		std::getline(screenFile, _name);
		if (_screenWidth < MIN_SCREEN_WIDTH || _screenHight < MIN_SCREEN_HIGHT || _timeLimit < MIN_PLAY_TIME || _timeLimit > MAX_PLAY_TIME || _name.empty()) {
			return;
		}

		_screen = std::make_unique<char[]>(_screenWidth * _screenHight);
		std::string line;
		for (auto lineNum = 0; lineNum < _screenHight; lineNum++)
		{
			if (!std::getline(screenFile, line)) {
				for (auto col = 0; col < _screenWidth; col++)
				{
					auto index = (lineNum * _screenWidth) + col;
					_screen[index] = EMPTY_TILE;
				}
				continue;
			}
			auto lineLength = line.size();
			for (auto col = 0; col < _screenWidth; col++)
			{
				auto index = (lineNum * _screenWidth) + col;
				if (col < lineLength) {
					_screen[index] = line[col];
					continue;
				}

				_screen[index] = EMPTY_TILE;
			}
		}
	}
	void CreateBlocksFromScreenChars() {
		if (!IsLoaded()) {
			return;
		}

		for (auto row = 0; row < _screenHight; row++)
		{
			for (auto col = 0; col < _screenWidth; col++)
			{
				auto index = (row * _screenWidth) + col;
				if (_screen[index] < A_BLOCK_NAME || _screen[index] > Z_BLOCK_NAME) {
					continue;
				}

				Point point(col, (_screenHight-1) - row);
				char name = _screen[index];

				auto hasSameName = [name](const Block& block) {
					return block.Is(name);
					};
				auto blockIter = std::find_if(_blocks.begin(), _blocks.end(), hasSameName);
				if (blockIter != _blocks.end()) {
					blockIter->AddPosition(point);
					continue;
				}

				Block block(name, this);
				block.AddPosition(point);
				_blocks.push_back(block);
			}
		}
	}
	void CreateShipsFromScreenChars() {
		if (!IsLoaded()) {
			return;
		}

		for (auto row = 0; row < _screenHight; row++)
		{
			for (auto col = 0; col < _screenWidth; col++)
			{
				auto index = (row * _screenWidth) + col;
				if (_screen[index] != BIG_SHIP_NAME && _screen[index] != SMALL_SHIP_NAME) {
					continue;
				}

				Point point(col, (_screenHight - 1) - row);
				char name = _screen[index];

				auto hasSameName = [name](const Ship& ship) {
					return ship.Is(name);
					};

				auto shipIter = std::find_if(_ships.begin(), _ships.end(), hasSameName);
				if (shipIter != _ships.end()) {
					shipIter->AddPosition(point);
					continue;
				}

				Ship ship(name, this);
				ship.AddPosition(point);
				if (name == BIG_SHIP_NAME) {
					ship.Activete();
				}
				_ships.push_back(ship);
			}
		}
	}
	void CreateExitsFromScreenChars() {
		if (!IsLoaded()) {
			return;
		}

		for (auto row = 0; row < _screenHight; row++)
		{
			for (auto col = 0; col < _screenWidth; col++)
			{
				auto index = (row * _screenWidth) + col;
				if (_screen[index] < ZERO_EXIT_NAME || _screen[index] > NINE_EXIT_NAME) {
					continue;
				}

				Point point(col, (_screenHight - 1) - row);
				char name = _screen[index];

				auto hasSameName = [name](const Exit& exit) {
					return exit.Is(name);
					};

				auto exitIter = std::find_if(_exits.begin(), _exits.end(), hasSameName);
				if (exitIter != _exits.end()) {
					exitIter->AddPosition(point);
					continue;
				}

				Exit exit(name, this);
				exit.AddPosition(point);
				_exits.push_back(exit);
			}
		}
	}
	bool IsValid() const {
		if (_ships.size() != 2 || _exits.size() < 1) {
			return false;
		}

		for (const auto& ship : _ships)
		{
			if (!ship.IsValid())
				return false;
		}

		auto numOfBigExits = 0;
		for (const auto& exit : _exits)
		{
			if (!exit.IsValid())
				return false;

			if (exit.CanFitBigShip()) {
				numOfBigExits++;
			}
		}

		return numOfBigExits > 0;
	}
	void ClearScreen() {
		_screen.reset();
		_screenWidth = 0;
		_screenHight = 0;
		_timeLimit = 0;
		_blocks.clear();
		_ships.clear();
	}
	void DrawWalls(IPainter& painter) const {
		for (auto row = 0; row < _screenHight; row++)
		{
			for (auto col = 0; col < _screenWidth; col++)
			{
				auto index = (row * _screenWidth) + col;
				if (_screen[index] == WALL_TILE) {
					Point wallPoint(col, (_screenHight -1) - row);
					painter.Draw(wallPoint, WALL_TILE);
				}
			}
		}
	}
	void DrawExits(IPainter& painter) const {
		for (const auto& exit : _exits)
		{
			exit.Draw(painter);
		}
	}
	void DrawBlocks(IPainter& painter) const {
		for (const auto& block : _blocks)
		{
			block.Draw(painter);
		}
	}
	void DrawShips(IPainter& painter) const {
		for (const auto& ship : _ships)
		{
			ship.Draw(painter);
		}
	}
	void SortBlocks() {
		auto byLowerFirst = [](const Block& a, const Block& b) {
			auto& aLowestPoint = a.GetLowestPoint();
			auto& bLowestPoint = b.GetLowestPoint();
			if (aLowestPoint.Y < bLowestPoint.Y) {
				return true;
			}

			if (aLowestPoint.Y == bLowestPoint.Y && aLowestPoint.X < bLowestPoint.X) {
				return true;
			}

			return false;
			};
		std::sort(_blocks.begin(), _blocks.end(), byLowerFirst);
	}
	void SortShips() {
		auto byLowerFirst = [](const Ship& a, const Ship& b) {
			auto& aLowestPoint = a.GetLowestPoint();
			auto& bLowestPoint = b.GetLowestPoint();
			if (aLowestPoint.Y < bLowestPoint.Y) {
				return true;
			}

			if (aLowestPoint.Y == bLowestPoint.Y && aLowestPoint.X < bLowestPoint.X) {
				return true;
			}

			return false;
			};
		std::sort(_ships.begin(), _ships.end(), byLowerFirst);
	}
	void ReEnterEntitiesInOrder() {
		_entities.clear();
		for (auto i = 0; i < _ships.size(); i++)
		{
			_entities.push_back((GameObject*)(_ships.data() + i));
		}
		for (auto i = 0; i < _blocks.size(); i++)
		{
			_entities.push_back((GameObject*)(_blocks.data() + i));
		}
	}
	void SwapShipsOnInput(const IController& controller) {
		auto switchToBig = controller.GetButtonState(Button::SwitchToBigShip).Pressed;
		auto switchToSmall = controller.GetButtonState(Button::SwitchToSmallShip).Pressed;

		if (switchToBig && switchToSmall) {
			return;
		}

		if (!switchToBig && !switchToSmall) {
			return;
		}

		if (_ships.size() == 1) {
			if (_activeShipName == SMALL_SHIP_NAME && switchToSmall) {
				return;
			}

			if (_activeShipName == BIG_SHIP_NAME && switchToBig) {
				return;
			}

			_ships.begin()->Deactivete();
			_ships.begin()->Activete();
			return;
		}

		if (_activeShipName == BIG_SHIP_NAME && switchToSmall) {
			_activeShipName = SMALL_SHIP_NAME;
			SwitchToShip(SMALL_SHIP_NAME);
			return;
		}
		
		if (_activeShipName == SMALL_SHIP_NAME && switchToBig) {
			_activeShipName = BIG_SHIP_NAME;
			SwitchToShip(BIG_SHIP_NAME);
		}
	}
	void SwitchToShip(const char name) {
		for (auto& ship : _ships) {
			if (ship.Is(name)) {
				ship.Activete();
			}
			else {
				ship.Deactivete();
			}
		}
	}
	bool ShipInsideExit(Ship& ship, Exit& exit) const {
		auto& shipPositions = ship.GetPositions();
		auto& exitPositions = exit.GetPositions();
		for (auto& point : shipPositions) {
			if (!contains(exitPositions, point)) {
				return false;
			}
		}
		return true;
	}
	void RemoveShipsInExit(Exit& exit) {
		auto shipIsInsideExit = [&](Ship& ship) 
			{ 
				if (ShipInsideExit(ship, exit)) {
					return true;
				}
				return false; 
			};
		auto shipToRemove = std::remove_if(_ships.begin(), _ships.end(), shipIsInsideExit);
		_ships.erase(shipToRemove, _ships.end());
	}
};