#pragma once
#include <functional>
#include <filesystem>

#include "../Extensions.h"

#include "UIStack.h"
#include "UIElement.h"
#include "../Screens/Screen.h"

constexpr auto LEVELS_FOLDER = "Assets/Screens";

class ScreenSelector : public UIElement {
private:
	std::function<void(Screen& screen)> _screenSelected;
	UIStack& _menuStack;
	int _screenIndex;
	int _maxLength = 0;
	std::vector<Screen> _screens;
	std::vector<std::string> _screensLines;
	std::string _title = "Select Screen:";
	std::string _controles = "(ESC) <- Back | (W) ^ Up | (X) v Down | (S) - Select";
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

		_maxLength = _controles.size();
		for(auto i = 0; i < _screens.size(); i++)
		{
			auto screenLine = std::format("({}) - {}", (i + 1), _screens[i].Name());
			_screensLines.push_back(screenLine);
			if (screenLine.size() > _maxLength) {
				_maxLength = screenLine.size();
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
		auto startX = (screenWidth / 2) - (_maxLength / 2);
		auto startY = (screenHight / 4);
		auto endY = startY * 3;

		renderer.Fill(startX - INFO_PADING, startY - INFO_PADING, startX + _maxLength + INFO_PADING, endY, '\0', FG_BLACK | BG_BLUE);
		renderer.DrawString(startX, startY, _title, FG_BLACK | BG_BLUE);

		for (auto i = 0; i < _screens.size(); i++)
		{
			auto color = FG_BLACK | BG_BLUE;
			if (_screenIndex == i) {
				color = color | COMMON_LVB_REVERSE_VIDEO;
			}

			renderer.DrawString(startX, startY + i + 1, _screensLines[i], color);
		}

		renderer.DrawString(startX, endY - 1, _controles, FG_BLACK | BG_BLUE);
	}
};