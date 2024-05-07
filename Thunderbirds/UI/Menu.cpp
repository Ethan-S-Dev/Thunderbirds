#include "Menu.h"
#include "../Rendering/Colors.h"

#define UPPER_LINE 0x0400

Menu::Menu(std::function<void(Screen& screen)> screenSelected,std::function<void()> onNewGamePressed,std::function<void()> onExitPressed,UIStack& menuStack) :
	_onNewGamePressed(onNewGamePressed),
	_onExitPressed(onExitPressed),
	_info("Assets/GameInfo/thunderbirds.info", menuStack),
	_screenSelector(menuStack, screenSelected),
	_menuStack(menuStack),
	_screenSelected(false)
{}

bool Menu::Init() {
	if (!_info.Load()) {
		return false;
	}

	if (!_screenSelector.Load()) {
		return false;
	}

	return true;
}

void Menu::Update(float delta, const IController& controller) {
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

void Menu::Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
	std::string menu = "        Thunderbirds!        ";
	std::string back = "  (ESC) Resume Game          ";
	std::string newGame = "    (1) New Game             ";
	std::string levels = "    (2) Screen Selection     ";
	std::string info = "    (8) Instractions         ";
	std::string exit = "    (9) Exit                 ";

	auto menuStartX = (screenWidth / 2) - (menu.size() / 2);
	auto menuStartY = (screenHight / 2) - (screenHight / 4);

	auto selectedGameColor = _screenSelected ? FG_BLACK : FG_DARK_GREY;

	renderer.DrawString(menuStartX, menuStartY, menu, FG_DARK_RED | BG_BLUE);
	renderer.DrawString(menuStartX, menuStartY + 1, back, selectedGameColor | BG_BLUE);
	renderer.DrawString(menuStartX, menuStartY + 2, newGame, selectedGameColor | BG_BLUE);
	renderer.DrawString(menuStartX, menuStartY + 3, levels, FG_BLACK | BG_BLUE);
	renderer.DrawString(menuStartX, menuStartY + 4, info, FG_BLACK | BG_BLUE);
	renderer.DrawString(menuStartX, menuStartY + 5, exit, FG_BLACK | BG_BLUE);
}

void Menu::SetScreenSelected(bool selected) {
	_screenSelected = selected;
}

void Menu::OnNewGamePressed() {
	_onNewGamePressed();
}

void Menu::OnExitPressed() {
	_onExitPressed();
}