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
public:
	GameInfo(const std::string fileName, UIStack& menuStack) :
		_menuStack(menuStack), _fileName(fileName), _gameInfoText()
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
		auto maxLength = 0;
		for (auto& line : _gameInfoText)
		{
			if (line.size() > maxLength) {
				maxLength = line.size();
			}
		}

		auto menuStartX = (screenWidth / 2) - (maxLength / 2);
		auto menuStartY = (screenHight / 2) - (screenHight / 4);

		renderer.Fill(menuStartX - INFO_PADING, menuStartY - INFO_PADING, menuStartX + maxLength + INFO_PADING, menuStartY + _gameInfoText.size() + INFO_PADING, '\0', BG_BLUE);
		auto lineNum = 0;
		for (auto& line : _gameInfoText)
		{
			renderer.DrawString(menuStartX, menuStartY + (lineNum++), line, FG_BLACK | BG_BLUE);
		}
		std::string back = "(ESC) <- Back";
		renderer.DrawString(menuStartX, menuStartY + (lineNum), back, FG_BLACK | BG_BLUE);
	}
};