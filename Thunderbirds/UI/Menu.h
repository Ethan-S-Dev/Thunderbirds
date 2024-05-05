#pragma once
#include <functional>

#include "../Rendering/Colors.h"
#include "../Rendering/Rect.h"
#include "../Extensions.h"
#include "UIStack.h"
#include "UIElement.h"
#include "GameInfo.h"
#include "ScreenSelector.h"

class Menu : public UIElement {
private:
	std::function<void(bool paused)> _onPausePressed;
	std::function<void()> _onNewGamePressed;
	std::function<void()> _onExitPressed;

	GameInfo _info;
	ScreenSelector _screenSelector;
	UIStack& _menuStack;
public:
	Menu(
		std::function<void(Screen& screen)> screenSelected,
		std::function<void()> onNewGamePressed,
		std::function<void()> onExitPressed,
		UIStack& menuStack
	) :
		_onNewGamePressed(onNewGamePressed), 
		_onExitPressed(onExitPressed),
		_info("thunderbirds.info", menuStack), 
		_screenSelector(menuStack, screenSelected),
		_menuStack(menuStack)
	{}
public:
	bool Init() {
		if (!_info.Load()) {
			return false;
		}

		if (!_screenSelector.Load()) {
			return false;
		}

		return true;
	}
	void Update(float delta, const IController& controller) {
		if (controller.GetButtonState(Button::Exit).Pressed) {
			OnExitPressed();
			return;
		}

		if (controller.GetButtonState(Button::NewGame).Pressed) {
			OnNewGamePressed();
			return;
		}

		if (controller.GetButtonState(Button::Levels).Pressed) {
			_menuStack.Push(_screenSelector);
			return;
		}

		if (controller.GetButtonState(Button::Info).Pressed) {
			_menuStack.Push(_info);
			return;
		}
	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		std::string menu = "  Thunderbirds!        ";
		std::string newGame = "  (1) New Game         ";
		std::string levels = "  (2) Screen Selection ";
		std::string info = "  (8) Instractions     ";
		std::string exit = "  (9) Exit             ";

		auto menuStartX = (screenWidth / 2) - (menu.size() / 2);
		auto menuStartY = (screenHight / 2) - (screenHight / 4);

		renderer.DrawString(menuStartX, menuStartY, menu, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 1, newGame, FG_BLACK | BG_BLUE | 0x0400);
		renderer.DrawString(menuStartX, menuStartY + 2, levels, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 3, info, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 4, exit, FG_BLACK | BG_BLUE);

	}
private:
	void OnNewGamePressed() {
		_onNewGamePressed();
	}
	void OnExitPressed() {
		_onExitPressed();
	}
};