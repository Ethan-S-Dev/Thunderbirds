#pragma once
#include <format>
#include <chrono>

#include "ConsoleGameEngine.h"
#include "Core/IPainter.h"
#include "Core/IController.h"
#include "Screens/Screen.h"

class Rect {
public:
	Point LeftDown;
	Point RightUp;

	Rect(Point leftDown, Point rightUp) : LeftDown(leftDown), RightUp(rightUp)
	{}
	Rect(int ldx, int ldy, int rux, int ruy) : LeftDown(ldx, ldy), RightUp(rux, ruy)
	{}
	Rect() : Rect(0, 0, 0, 0) {}
};

constexpr auto FREE_MOVE_WIDTH_RATIO = 1 / 6.f;
constexpr auto FREE_MOVE_HIGHT_RATIO = 3 / 12.f;

class Camera {
private:
	Point _cameraWorldFocusePoint;
	Rect _cameraConsoleRect;
	Point _cameraConsoleFucsePoint;
public:
	Rect CameraConsoleRect() const {
		return _cameraConsoleRect;
	}
	void Focuse(const Point freeMoveWorldFocusePoint) {
		auto focusePointXDif = freeMoveWorldFocusePoint.X - _cameraWorldFocusePoint.X;
		auto focusePointYDif = freeMoveWorldFocusePoint.Y - _cameraWorldFocusePoint.Y;
		auto maxXDif = ((_cameraConsoleRect.RightUp.X - _cameraConsoleRect.LeftDown.X) / 4) * 1;
		auto maxYDif = ((_cameraConsoleRect.LeftDown.Y - _cameraConsoleRect.RightUp.Y) / 4) * 1;

		if (abs(focusePointXDif) > maxXDif + 1) {
			_cameraWorldFocusePoint = freeMoveWorldFocusePoint;
			return;
		}

		if (abs(focusePointYDif) > maxYDif + 1) {
			_cameraWorldFocusePoint = freeMoveWorldFocusePoint;
			return;
		}

		if (focusePointXDif + maxXDif == -1) {
			_cameraWorldFocusePoint.X--;
		}
		if (focusePointXDif - maxXDif == 1) {
			_cameraWorldFocusePoint.X++;
		}
		if (focusePointYDif - maxYDif == 1) {
			_cameraWorldFocusePoint.Y++;
		}
		if (focusePointYDif + maxYDif == -1) {
			_cameraWorldFocusePoint.Y--;
		}
	}
	Point TransformToConsole(const Point pointInWorld) const {
		auto centerXdif = _cameraWorldFocusePoint.X - _cameraConsoleFucsePoint.X;
		auto centerYdif = (_cameraConsoleRect.LeftDown.Y - _cameraWorldFocusePoint.Y) - _cameraConsoleFucsePoint.Y;

		Point pointInConsole(pointInWorld.X - centerXdif, (_cameraConsoleRect.LeftDown.Y - pointInWorld.Y) - centerYdif);

		if (
			pointInConsole.X < _cameraConsoleRect.LeftDown.X ||
			pointInConsole.X > _cameraConsoleRect.RightUp.X - 1 ||
			pointInConsole.Y < _cameraConsoleRect.RightUp.Y ||
			pointInConsole.Y > _cameraConsoleRect.LeftDown.Y) {
			pointInConsole.X = -1;
			pointInConsole.Y = -1;
		}
		return pointInConsole;
	}
	void SetCameraConsoleRectangle(Point consoleScreenSize, int topMargin) {
		_cameraConsoleRect.LeftDown.X = 0;
		_cameraConsoleRect.LeftDown.Y = consoleScreenSize.Y;
		_cameraConsoleRect.RightUp.X = consoleScreenSize.X;
		_cameraConsoleRect.RightUp.Y = topMargin;

		_cameraConsoleFucsePoint.X = (_cameraConsoleRect.RightUp.X - _cameraConsoleRect.LeftDown.X) / 2;
		_cameraConsoleFucsePoint.Y = (_cameraConsoleRect.LeftDown.Y - _cameraConsoleRect.RightUp.Y) / 2;
	}
	int Hight() const {
		return _cameraConsoleRect.LeftDown.Y - _cameraConsoleRect.RightUp.Y;
	}
	int Width() const {
		return _cameraConsoleRect.RightUp.X - _cameraConsoleRect.LeftDown.X;
	}
};

constexpr auto CAMERA_TOP_MARGIN = 1;
constexpr auto CAMERA_FRAME_COLOR = BG_DARK_BLUE;

class Thunderbirds : public ConsoleGameEngine, public IPainter {
private:
	class ProcessController : public IController {
	private:
		const Thunderbirds* _thunderBirds;
	public:
		ProcessController(const Thunderbirds* thunderBirds) :
			_thunderBirds(thunderBirds)
		{}
	public:
		ButtonState IController::GetButtonState(Button btn) const {
			return _thunderBirds->GetProcessButtonState(btn);
		}
	};
	class PhysicsController : public IController {
	private:
		const Thunderbirds* _thunderBirds;
	public:
		PhysicsController(const Thunderbirds* thunderBirds) :
			_thunderBirds(thunderBirds)
		{}
	public:
		ButtonState IController::GetButtonState(Button btn) const {
			return _thunderBirds->GetPhysicsButtonState(btn);
		}
	};

private:
	Screen _screen;
	VKCode _buttonMapping[(short)Button::NumOfButtons];
	VKCode _physicsButtonMapping[(short)Button::NumOfButtons];
	bool _isPaused;
	bool _showInfo;
	int _lifePointsLeft;
	Camera _camera;
	ProcessController _processController;
	PhysicsController _physicsController;
public:
	Thunderbirds() :
		_screen("Screens/level_5.screen"), 
		_buttonMapping{
		VKCode::WKey,
		VKCode::XKey,
		VKCode::AKey,
		VKCode::DKey,
		VKCode::BKey,
		VKCode::SKey,
		VKCode::OneKey,
		VKCode::EightKey,
		VKCode::NineKey,
		VKCode::EscKey,
		},
		_physicsButtonMapping{
		VKCode::WKey,
		VKCode::XKey,
		VKCode::AKey,
		VKCode::DKey,
		VKCode::BKey,
		VKCode::SKey,
		VKCode::OneKey,
		VKCode::EightKey,
		VKCode::NineKey,
		VKCode::EscKey,
		},
		_isPaused(false),
		_showInfo(false),
		_lifePointsLeft(0),
		_camera(),
		_processController(this),
		_physicsController(this)
	{
		_appName = "Thunderbirds";
	}
public:
	void IPainter::Draw(const Point& point, char c) {
		auto pointInConsole = _camera.TransformToConsole(point);
		int color = FG_WHITE;
		if (pointInConsole.X == 0 || pointInConsole.X == _screenSize.X - 1 ||
			pointInConsole.Y == CAMERA_TOP_MARGIN || pointInConsole.Y == _screenSize.Y - 1) {
			color = color | CAMERA_FRAME_COLOR;
		}
		ConsoleGameEngine::Draw(pointInConsole.X, pointInConsole.Y, c, color);
	}
public:
	ButtonState GetProcessButtonState(Button btn) const {
		auto key = _buttonMapping[(short)btn];
		auto keyState = _processKeys[key];
		ButtonState btnState(keyState.Pressed, keyState.Released, keyState.Held);
		return btnState;
	}
	ButtonState GetPhysicsButtonState(Button btn) const {
		auto key = _buttonMapping[(short)btn];
		auto keyState = _physicsKeys[key];
		ButtonState btnState(keyState.Pressed, keyState.Released, keyState.Held);
		return btnState;
	}
private:
	void ConsoleGameEngine::OnCreate() {
		if (!_screen.IsLoaded()) {
			Stop();
			return;
		}

		_lifePointsLeft = _screen.StartingLifePoints();
		Point consoleScreenSize(_screenSize.X, _screenSize.Y);
		_camera.SetCameraConsoleRectangle(consoleScreenSize, CAMERA_TOP_MARGIN);
		SetCameraFocusePoint();
	}
	void ConsoleGameEngine::OnPhysicsUpdate(float elapsedTime) {
		if (_screen.State() == ScreenState::Playing && !_isPaused) {
			_screen.PhysicsUpdate(elapsedTime, _physicsController);
			SetCameraFocusePoint();
		}
	}
	void ConsoleGameEngine::OnUpdate(float elapsedTime) {
		if (GetProcessButtonState(Button::Pause).Pressed) {
			_isPaused = !_isPaused;
			if (!_isPaused) {
				_showInfo = false;
			}
		}

		if (_screen.State() == ScreenState::Playing && !_isPaused) {
			_screen.Update(elapsedTime, _processController);
		}

		Clear();
		DrawGameInfo();
		DrawCameraFrame();
		_screen.Draw(*this);

		if (_screen.State() == ScreenState::Lost) {
			_lifePointsLeft--;
			if (_lifePointsLeft > 0) {
				_screen.Reset();
			}
			else {
				ShowLostMessage();
			}
		}

		if (_screen.State() == ScreenState::Won) {
			ShowWonMessage();
		}

		if (_isPaused) {
			_screen.GroundShips();
			ShowMenu();
			auto exit = GetProcessButtonState(Button::Exit).Pressed;
			if (exit) {
				Clear();
				Stop();
				return;
			}
			auto startNewGame = GetProcessButtonState(Button::NewGame).Pressed;
			if (startNewGame) {
				_lifePointsLeft = _screen.StartingLifePoints();
				_screen.Reset();
				_isPaused = false;
				_showInfo = false;
				return;
			}

			if (GetProcessButtonState(Button::Info).Pressed) {
				_showInfo = !_showInfo;
			}

			if (_showInfo) {
				ShowInfo();
			}
		}
	}
	void OnScreenResize() {
		Point consoleScreenSize(_screenSize.X, _screenSize.Y);
		_camera.SetCameraConsoleRectangle(consoleScreenSize, CAMERA_TOP_MARGIN);
	}
private:
	void DrawGameInfo() {
		std::string lifes;
		for (auto i = 0; i < _lifePointsLeft; i++)
		{
			lifes += "<3";
		}
		for (auto i = 0; i < _screen.StartingLifePoints() - _lifePointsLeft; i++)
		{
			lifes += "  ";
		}
		auto total_seconds_left = _screen.SecondsLeftToFinishScreen();
		auto minutes = total_seconds_left / 60;
		auto seconds = total_seconds_left % 60;

		std::string infoBar = std::format("| Ship: {}, Time: {:02}:{:02}, Life: ", _screen.ActiveShipName(), minutes, seconds, lifes);
		std::string end = " |";
		auto sumLengths = infoBar.size() + lifes.size() + end.size();
		DrawString((_screenSize.X / 2) - (sumLengths / 2), 0, infoBar);
		DrawString((_screenSize.X / 2) - (sumLengths / 2) + infoBar.size(), 0, lifes, FG_RED);
		DrawString((_screenSize.X / 2) - (sumLengths / 2) + infoBar.size() + lifes.size(), 0, end);
	}
	void DrawCameraFrame() {
		auto frame = _camera.CameraConsoleRect();
		for (auto i = frame.LeftDown.X; i < frame.RightUp.X; i++)
		{
			ConsoleGameEngine::Draw(i, frame.RightUp.Y, ' ', CAMERA_FRAME_COLOR);
			ConsoleGameEngine::Draw(i, frame.LeftDown.Y - 1, ' ', CAMERA_FRAME_COLOR);
		}

		for (auto i = frame.RightUp.Y ; i < frame.LeftDown.Y; i++)
		{
			ConsoleGameEngine::Draw(frame.RightUp.X - 1, i, ' ', CAMERA_FRAME_COLOR);
			ConsoleGameEngine::Draw(frame.LeftDown.X, i, ' ', CAMERA_FRAME_COLOR);
		}
	}
	void ShowMenu() {
		std::string menu = "this is the menu";
		DrawString((_screenSize.X / 2) - (menu.size() / 2), (_screenSize.Y / 2) - (_screenSize.Y / 4), menu, BG_BLUE);
	}
	void ShowWonMessage() {
		std::string menu = "You won!";
		DrawString((_screenSize.X / 2) - (menu.size() / 2), (_screenSize.Y / 2) - (_screenSize.Y / 4), menu, BG_GREY | FG_YELLOW);
	}
	void ShowLostMessage() {
		std::string menu = "You lost!";
		DrawString((_screenSize.X / 2) - (menu.size() / 2), (_screenSize.Y / 2) - (_screenSize.Y / 4), menu, BG_DARK_RED | FG_WHITE);
	}
	void ShowInfo() {
		std::string info = "This is info";
		DrawString((_screenSize.X / 2) - (info.size() / 2), (_screenSize.Y / 2) - (_screenSize.Y / 4) + 1, info, BG_BLUE);
	}
	void SetCameraFocusePoint() {
		auto focusePoint = _screen.ActiveShipMainPoint();
		_camera.Focuse(focusePoint);
	}
};