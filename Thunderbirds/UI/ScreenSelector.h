#pragma once
#include <functional>
#include <filesystem>

#include "UIStack.h"
#include "UIElement.h"
#include "../Screens/Screen.h"

constexpr auto LEVELS_FOLDER = "Levels";

class ScreenSelector : public UIElement {
private:
	std::function<void(Screen& screen)> _screenSelected;
	UIStack& _menuStack;
	int _screenIndex;
	std::vector<Screen> _screens;
public:
	ScreenSelector(UIStack& menuStack, std::function<void(Screen& screen)> screenSelected) :_screenSelected(screenSelected), _menuStack(menuStack), _screenIndex(0)
	{}
	bool Load() {
		for (const auto& entry : std::filesystem::directory_iterator(LEVELS_FOLDER)) {
			auto ext = entry.path().extension().string();
			if (ext != ".screen") {
				continue;
			}

			_screens.push_back({ entry.path().string() });

			if (!_screens.back().IsLoaded()) {
				return false;
			}
		}

		return true;
	}
	void Update(float delta, const IController& controller) {
		if (controller.GetButtonState(Button::Up).Pressed) {
			_screenIndex--;
		}

		if (controller.GetButtonState(Button::Down).Pressed) {
			_screenIndex++;
		}

		if (_screenIndex < 0) {
			_screenIndex = _screens.size() - 1;
		}

		if (_screenIndex >= _screens.size()) {
			_screenIndex = 0;
		}

		if (controller.GetButtonState(Button::SwitchToSmallShip).Pressed) {
			_screenSelected(_screens[_screenIndex]);
		}
	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		std::string title = "Select Screen:";
		std::string controles = "(ESC) <- Back | (W) ^ Up | (X) v Down | (S) - Select";
		auto maxLength = controles.size();
		for (auto& screen : _screens)
		{
			if (screen.Name().size() > maxLength) {
				maxLength = screen.Name().size();
			}
		}

		auto startX = (screenWidth / 2) - (maxLength / 2);
		auto startY = (screenHight / 2) - (screenHight / 4);

		renderer.Fill(startX - INFO_PADING, startY - INFO_PADING, startX + maxLength + INFO_PADING, startY + _screens.size() + 2 + INFO_PADING, '\0', FG_BLACK | BG_BLUE);
		renderer.DrawString(startX, startY, title, FG_BLACK | BG_BLUE);
		for (auto i = 0; i < _screens.size(); i++)
		{
			auto color = FG_BLACK | BG_BLUE;
			if (_screenIndex == i) {
				color = color | COMMON_LVB_REVERSE_VIDEO;
			}

			renderer.DrawString(startX, startY + i + 1, _screens[i].Name(), color);
		}

		renderer.DrawString(startX, startY + _screens.size() + 1, controles, FG_BLACK | BG_BLUE);
	}
};