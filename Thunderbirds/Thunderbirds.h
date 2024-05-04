#pragma once
#include <format>
#include <chrono>

#include "Extensions.h"
#include "ConsoleGameEngine.h"
#include "Core/IPainter.h"
#include "Core/IController.h"
#include "Screens/Screen.h"
#include "Rendering/Colors.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Camera.h"
#include "UI/Menu.h"

constexpr auto CAMERA_TOP_MARGIN = 1;
constexpr auto CAMERA_FRAME_COLOR = BG_DARK_BLUE;

class Thunderbirds : public ConsoleGameEngine, public IPainter, public IRenderer {
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
	int _lifePointsLeft;
	Camera _camera;
	Menu _menu;
	ProcessController _processController;
	PhysicsController _physicsController;
public:
	Thunderbirds() :
		_screen("Levels/level_4.screen"), 
		_buttonMapping{
		VKCode::WKey,
		VKCode::XKey,
		VKCode::AKey,
		VKCode::DKey,
		VKCode::BKey,
		VKCode::SKey,
		VKCode::OneKey,
		VKCode::TwoKey,
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
		VKCode::TwoKey,
		VKCode::EightKey,
		VKCode::NineKey,
		VKCode::EscKey,
		},
		_isPaused(false),
		_lifePointsLeft(0),
		_camera(),
		_menu(
			std::bind(&Thunderbirds::OnPausedPressed, this,std::placeholders::_1),
			std::bind(&Thunderbirds::OnNewGamePressed, this),
			std::bind(&Thunderbirds::OnExitPressed, this)),
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

		if (!_menu.Init()) {
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
		}

		_menu.Draw(_screenSize.X, _screenSize.Y, *this);
		_menu.Update(elapsedTime, _processController);
	}
	void OnScreenResize() {
		Point consoleScreenSize(_screenSize.X, _screenSize.Y);
		_camera.SetCameraConsoleRectangle(consoleScreenSize, CAMERA_TOP_MARGIN);
	}
public:
	void IRenderer::Clear() {
		ConsoleGameEngine::Clear();
	}
	void IRenderer::Draw(int x, int y, char c = 0xDB, short col = 0x000F) {
		ConsoleGameEngine::Draw(x, y, c, col);
	}
	void IRenderer::Fill(int x1, int y1, int x2, int y2, char c = 0xDB, short col = 0x000F) {
		ConsoleGameEngine::Fill(x1, y1, x2, y2, c, col);
	}
	void IRenderer::DrawString(int x, int y, std::string s, short col = 0x000F) {
		ConsoleGameEngine::DrawString(x, y, s, col);
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
	short atr = 0x0000;
	int count = 0;
	void ShowWonMessage() {
		std::string menu = "You won!";
		DrawString((_screenSize.X / 2) - (menu.size() / 2), (_screenSize.Y / 2) - (_screenSize.Y / 4), menu, BG_GREY | FG_YELLOW);
	}
	void ShowLostMessage() {
		std::string menu = "You lost!";
		DrawString((_screenSize.X / 2) - (menu.size() / 2), (_screenSize.Y / 2) - (_screenSize.Y / 4), menu, BG_DARK_RED | FG_WHITE);
	}
	void SetCameraFocusePoint() {
		auto focusePoint = _screen.ActiveShipMainPoint();
		_camera.Focuse(focusePoint);
	}
private:
	void OnPausedPressed(bool paused) {
		_isPaused = paused;
	}
	void OnNewGamePressed() {
		_lifePointsLeft = _screen.StartingLifePoints();
		_screen.Reset();
		_isPaused = false;
	}
	void OnExitPressed() {
		Clear();
		Stop();
	}
};