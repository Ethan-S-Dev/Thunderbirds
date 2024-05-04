#pragma once
#include <functional>

#include "../Rendering/Colors.h"
#include "../Rendering/Rect.h"
#include "../Extensions.h"
#include "UIElement.h"

class Menu : public UIElement {
private:
	std::function<void(bool paused)> _onPausePressed;
	std::function<void()> _onNewGamePressed;
	std::function<void()> _onExitPressed;

	Rect _frame;
	bool _isPaused;
	bool _showInfo;
	bool _showLevels;
public:
	Menu(Rect frame, std::function<void(bool)> onPausePressed, std::function<void()> onNewGamePressed, std::function<void()> onExitPressed) :
		_onPausePressed(onPausePressed), _onNewGamePressed(onNewGamePressed), _onExitPressed(onExitPressed),
		_frame(frame),
		_isPaused(false), _showInfo(false), _showLevels(false)
	{}
public:
	void SetFrame(Rect newFrame) {
		_frame = newFrame;
	}
	void Update(float delta, const IController& controller) {
		if (controller.GetButtonState(Button::Pause).Pressed) {
			_isPaused = !_isPaused;
			if (!_isPaused) {
				_showInfo = false;
			}
			_onPausePressed(_isPaused);
		}

		if (!_isPaused) {
			return;
		}

		if (controller.GetButtonState(Button::Exit).Pressed) {
			_onExitPressed();
			return;
		}

		if (controller.GetButtonState(Button::NewGame).Pressed) {
			_isPaused = false;
			_showInfo = false;
			_showLevels = false;
			_onNewGamePressed();
			return;
		}

		if (controller.GetButtonState(Button::Info).Pressed) {
			_showInfo = !_showInfo;
			_showLevels = _showLevels ? !_showInfo : false;
			return;
		}

		if (controller.GetButtonState(Button::Levels).Pressed) {
			_showLevels = !_showLevels;
			_showInfo = _showInfo ? !_showLevels : false;
			return;
		}
	}
	void Draw(float delta, IRenderer& renderer) const{
		if (!_isPaused) {
			return;
		}

		ShowMenu(renderer);
		if (_showInfo) {
			ShowInfo(renderer);
		}
		if (_showLevels) {
			ShowLevels(renderer);
		}
	}
	void ShowMenu(IRenderer& renderer) const{
		std::string menu = "  Thunderbirds!    ";
		std::string newGame = "  (1) New Game     ";
		std::string levels = "  (2) Levels       ";
		std::string info = "  (8) Information  ";
		std::string exit = "  (9) Exit         ";
		auto width = _frame.RightUp.X - _frame.LeftDown.X;
		auto hight = _frame.LeftDown.Y - _frame.RightUp.Y;
		auto menuStartX = (width / 2) - (menu.size() / 2);
		auto menuStartY = (hight / 2) - (hight / 4);

		renderer.DrawString(menuStartX, menuStartY, menu, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 1, newGame, FG_BLACK | BG_BLUE | 0x0400);
		renderer.DrawString(menuStartX, menuStartY + 2, levels, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 3, info, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 4, exit, FG_BLACK | BG_BLUE);
	}
	void ShowInfo(IRenderer& renderer) const{
		std::string info = "This is info";
		auto width = _frame.RightUp.X - _frame.LeftDown.X;
		auto hight = _frame.LeftDown.Y - _frame.RightUp.Y;
		auto infoStartX = (width / 2) - (info.size() / 2);
		auto infoStartY = (hight / 2) - (hight / 4);
		renderer.DrawString(infoStartX, infoStartY, info, BG_BLUE);
	}
	void ShowLevels(IRenderer& renderer) const{
		std::string levels = "This is Levels";
		auto width = _frame.RightUp.X - _frame.LeftDown.X;
		auto hight = _frame.LeftDown.Y - _frame.RightUp.Y;
		auto levelsStartX = (width / 2) - (levels.size() / 2);
		auto levelsStartY = (hight / 2) - (hight / 4);
		renderer.DrawString(levelsStartX, levelsStartY, levels, BG_BLUE);
	}
};