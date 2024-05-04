#pragma once
#include <functional>

#include "../Rendering/Colors.h"
#include "../Rendering/Rect.h"
#include "../Extensions.h"
#include "UIElement.h"

constexpr auto INFO_PADING = 2;

class GameInfo : public UIElement {
	const std::string _fileName;
	std::vector<std::string> _gameInfoText;
	bool _active;
public:
	GameInfo(const std::string fileName) :_fileName(fileName), _gameInfoText(), _active(false)
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
		if (controller.GetButtonState(Button::Info).Pressed) {
			_active = !_active;
		}
	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		if (!_active) {
			return;
		}

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
	}
	void SetActive(bool active) {
		_active = false;
	}
};

class Menu : public UIElement {
private:
	std::function<void(bool paused)> _onPausePressed;
	std::function<void()> _onNewGamePressed;
	std::function<void()> _onExitPressed;

	GameInfo _info;

	bool _isPaused;
	bool _showLevels;
public:
	Menu(std::function<void(bool)> onPausePressed, std::function<void()> onNewGamePressed, std::function<void()> onExitPressed) :
		_onPausePressed(onPausePressed), _onNewGamePressed(onNewGamePressed), _onExitPressed(onExitPressed),
		_info("thunderbirds.info"),
		_isPaused(false), _showLevels(false)
	{}
public:
	bool Init() {
		if (!_info.Load()) {
			return false;
		}

		return true;
	}
	void Update(float delta, const IController& controller) {
		if (controller.GetButtonState(Button::Pause).Pressed) {
			OnPausePressed();
		}

		if (!_isPaused) {
			return;
		}

		if (controller.GetButtonState(Button::Exit).Pressed) {
			OnExitPressed();
			return;
		}

		if (controller.GetButtonState(Button::NewGame).Pressed) {
			OnNewGamePressed();
			return;
		}

		if (controller.GetButtonState(Button::Levels).Pressed) {
			_showLevels = !_showLevels;
		}


		_info.Update(delta, controller);
	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		if (!_isPaused) {
			return;
		}

		ShowMenu(screenWidth, screenHight, renderer);
		_info.Draw(screenWidth, screenHight, renderer);
		
		if (_showLevels) {
			ShowLevels(screenWidth, screenHight, renderer);
		}
	}
private:
	void ShowMenu(int screenWidth, int screenHight, IRenderer& renderer) const {
		std::string menu = "  Thunderbirds!    ";
		std::string newGame = "  (1) New Game     ";
		std::string levels = "  (2) Levels       ";
		std::string info = "  (8) Information  ";
		std::string exit = "  (9) Exit         ";

		auto menuStartX = (screenWidth / 2) - (menu.size() / 2);
		auto menuStartY = (screenHight / 2) - (screenHight / 4);

		renderer.DrawString(menuStartX, menuStartY, menu, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 1, newGame, FG_BLACK | BG_BLUE | 0x0400);
		renderer.DrawString(menuStartX, menuStartY + 2, levels, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 3, info, FG_BLACK | BG_BLUE);
		renderer.DrawString(menuStartX, menuStartY + 4, exit, FG_BLACK | BG_BLUE);
	}
	void ShowLevels(int screenWidth, int screenHight, IRenderer& renderer) const {
		std::string levels = "This is Levels";

		auto levelsStartX = (screenWidth / 2) - (levels.size() / 2);
		auto levelsStartY = (screenHight / 2) - (screenHight / 4);
		renderer.DrawString(levelsStartX, levelsStartY, levels, BG_BLUE);
	}
	void OnPausePressed() {
		_isPaused = !_isPaused;
		_info.SetActive(false);
		_onPausePressed(_isPaused);
	}
	void OnNewGamePressed() {
		_isPaused = false;
		_showLevels = false;
		_info.SetActive(false);
		_onNewGamePressed();
	}
	void OnExitPressed() {
		_isPaused = false;
		_showLevels = false;
		_info.SetActive(false);
		_onExitPressed();
	}
};