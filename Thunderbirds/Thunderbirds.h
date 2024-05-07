#pragma once
#include <memory>

#include "Engine/ConsoleGameEngine.h"
#include "Core/IPainter.h"
#include "Core/IController.h"
#include "Rendering/IRenderer.h"
#include "Engine/VKCode.h"
#include "Screens/Screen.h"
#include "Rendering/Camera.h"
#include "UI/Menu.h"

class Thunderbirds : public ConsoleGameEngine, public IPainter, public IRenderer {
private:
	class ProcessController : public IController {
	private:
		const Thunderbirds* _thunderBirds;
	public:
		ProcessController(const Thunderbirds* thunderBirds);
	public:
		ButtonState IController::GetButtonState(Button btn) const;
	};
	class PhysicsController : public IController {
	private:
		const Thunderbirds* _thunderBirds;
	public:
		PhysicsController(const Thunderbirds* thunderBirds);
	public:
		ButtonState IController::GetButtonState(Button btn) const;
	};

private:
	Screen* _selectedScreen;
	VKCode _buttonMapping[(short)Button::NumOfButtons];
	VKCode _physicsButtonMapping[(short)Button::NumOfButtons];
	bool _isPaused;
	int _lifePointsLeft;
	Camera _camera;
	UIStack _menuStack;
	Menu _menu;
	ProcessController _processController;
	PhysicsController _physicsController;
public:
	Thunderbirds();
public:
	void IPainter::Draw(const Point& point, char c);
public:
	ButtonState GetProcessButtonState(Button btn) const;
	ButtonState GetPhysicsButtonState(Button btn) const;
private:
	void ConsoleGameEngine::OnCreate();
	void ConsoleGameEngine::OnPhysicsUpdate(float elapsedTime);
	void ConsoleGameEngine::OnUpdate(float elapsedTime);
	void OnScreenResize();
public:
	void IRenderer::Clear();
	void IRenderer::Draw(int x, int y, char c = 0xDB, short col = 0x000F);
	void IRenderer::Fill(int x1, int y1, int x2, int y2, char c = 0xDB, short col = 0x000F);
	void IRenderer::DrawString(int x, int y, const std::string& s, short col = 0x000F);
private:
	void DrawGameInfo();
	void DrawCameraFrame();
	void ShowWonMessage();
	void ShowLostMessage();
	void UpdateCameraFocusePoint();
	void UpdateScreenIfSelected(float delta);
	void DrawScreenIfSelected();
	void CheckAndShowScreenEndIfSelected();
	void GroundScreenShipsIfSelected();
	void HandleMenu(float delta);
private:
	void OnNewGamePressed();
	void OnExitPressed();
	void ScreenSelected(Screen& screen);
};