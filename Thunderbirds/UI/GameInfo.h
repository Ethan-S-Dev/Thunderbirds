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
public:
	GameInfo(const std::string fileName, UIStack& menuStack) :
		_menuStack(menuStack), _fileName(fileName), _gameInfoText(), _startLineIndex(0)
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
		return true;
	}
	void Update(float delta, const IController& controller) {

	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		std::string back = "(ESC) <- Back | (x) v Scroll Down | (w) ^ Scroll Up";
		auto maxLength = back.size();
		for (auto& line : _gameInfoText)
		{
			if (line.size() > maxLength) {
				maxLength = line.size();
			}
		}

		auto infoStartX = (screenWidth / 2) - (maxLength / 2);
		auto infoStartY = (screenHight / 2) - (screenHight / 4);
		auto infoEndY = (screenHight / 4) * 3;

		renderer.Fill(infoStartX - INFO_PADING, infoStartY - INFO_PADING, infoStartX + maxLength + INFO_PADING, infoEndY, '\0', BG_BLUE);
		auto lineNum = 0;
		auto numberOfVisableLines = min((infoEndY - INFO_PADING) - infoStartY, _gameInfoText.size());
		for (auto i = _startLineIndex; i < numberOfVisableLines || i < numberOfVisableLines; i++)
		{
			renderer.DrawString(infoStartX, infoStartY + i, _gameInfoText[i], FG_BLACK | BG_BLUE);
		}
		renderer.DrawString(infoStartX, infoEndY - INFO_PADING, back, FG_BLACK | BG_BLUE);
	}


};