#pragma once
#include "UIStack.h"

constexpr auto INFO_PADING = 2;

class GameInfo : public UIElement {
	UIStack& _menuStack;
	const std::string _fileName;
	std::vector<std::string> _gameInfoText;
	int _startLineIndex;
	int _numberOfVisableLines;
	std::string _back;
	int _maxLength;
public:
	GameInfo(const std::string fileName, UIStack& menuStack);
public:
	bool Load();
	void Update(float delta, const IController& controller);
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const;
private:
	void ConstHackedSetStartLineIndex(int index) const;
};