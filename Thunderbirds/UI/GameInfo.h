#pragma once
#include <functional>
#include <fstream>

#include "../Rendering/Colors.h"
#include "../Extensions.h"
#include "UIElement.h"

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
	GameInfo(const std::string fileName, UIStack& menuStack) :
		_menuStack(menuStack), _fileName(fileName), _gameInfoText(), _startLineIndex(0), _numberOfVisableLines(0),
		_back("(ESC) <- Back | (x) v Scroll Down | (w) ^ Scroll Up"), _maxLength(_back.size())
	{}

	bool Load() {
		std::ifstream infoFile(_fileName);
		if (!infoFile.is_open()) {
			return false;
		}

		std::string line;
		while (getline(infoFile, line)) {
			_gameInfoText.push_back(line);
		}

		infoFile.close();

		_numberOfVisableLines = _gameInfoText.size();

		_maxLength = _back.size();
		for (auto& line : _gameInfoText)
		{
			if (line.size() > _maxLength) {
				_maxLength = line.size();
			}
		}

		return true;
	}
	void Update(float delta, const IController& controller) {
		if (_startLineIndex < _gameInfoText.size() - 1) {
			if (controller.GetButtonState(Button::Down).Pressed) {
				_startLineIndex++;
			}
		}

		if (_startLineIndex > 0) {
			if (controller.GetButtonState(Button::Up).Pressed) {
				_startLineIndex--;
			}
		}
	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		auto infoStartX = (screenWidth / 2) - (_maxLength / 2);
		auto infoStartY = (screenHight / 2) - (screenHight / 4);
		auto infoEndY = (screenHight / 4) * 3;

		renderer.Fill(infoStartX - INFO_PADING, infoStartY - INFO_PADING, infoStartX + _maxLength + INFO_PADING, infoEndY, '\0', BG_BLUE);
		auto lineNum = 0;
		auto numberOfVisableLines = min((infoEndY - INFO_PADING) - infoStartY, _gameInfoText.size());

		if (numberOfVisableLines >= _gameInfoText.size()) {
			ConstHackedSetStartLineIndex(0);
		}

		auto maxIndex = min(numberOfVisableLines + _startLineIndex, _gameInfoText.size());

		if (_gameInfoText.size() - _startLineIndex < numberOfVisableLines) {
			ConstHackedSetStartLineIndex(_gameInfoText.size() - numberOfVisableLines);
		}

		for (auto i = _startLineIndex; i < maxIndex; i++)
		{
			renderer.DrawString(infoStartX, infoStartY + (lineNum++), _gameInfoText[i], FG_BLACK | BG_BLUE);
		}
		renderer.DrawString(infoStartX, infoEndY - 1, _back, FG_BLACK | BG_BLUE);
	}

private:
	void ConstHackedSetStartLineIndex(int index) const {
		int* hackedIndex = (int*)&_startLineIndex;
		(*hackedIndex) = index;
	}
};