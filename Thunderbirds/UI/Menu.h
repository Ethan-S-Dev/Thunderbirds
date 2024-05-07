#pragma once
#include "ScreenSelector.h"
#include "GameInfo.h"

class Menu : public UIElement {
private:
	std::function<void(bool paused)> _onPausePressed;
	std::function<void()> _onNewGamePressed;
	std::function<void()> _onExitPressed;
	GameInfo _info;
	ScreenSelector _screenSelector;
	UIStack& _menuStack;
	bool _screenSelected;
public:
	Menu(std::function<void(Screen& screen)> screenSelected, std::function<void()> onNewGamePressed, std::function<void()> onExitPressed, UIStack& menuStack);
public:
	bool Init();
	void Update(float delta, const IController& controller);
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const;
	void SetScreenSelected(bool selected);
private:
	void OnNewGamePressed();
	void OnExitPressed();
};