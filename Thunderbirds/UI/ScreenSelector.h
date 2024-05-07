#pragma once
#include <functional>

#include "UIStack.h"
#include "../Screens/Screen.h"

class ScreenSelector : public UIElement {
private:
	std::function<void(Screen& screen)> _screenSelected;
	UIStack& _menuStack;
	int _screenIndex;
	int _maxLength;
	std::vector<Screen> _screens;
	std::vector<std::string> _screensLines;
	std::string _title;
	std::string _controles;
public:
	ScreenSelector(UIStack& menuStack, std::function<void(Screen& screen)> screenSelected);
public:
	bool Load();
	void Update(float delta, const IController& controller);
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const;
};