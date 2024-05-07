#include "ScreenSelector.h"

ScreenSelector::ScreenSelector(UIStack& menuStack, std::function<void(Screen& screen)> screenSelected) :
	_screenSelected(screenSelected), 
	_menuStack(menuStack), 
	_screenIndex(0),
	_maxLength(0),
	_screens(),
	_screensLines(),
	_title("Select Screen:"),
	_controles("(ESC) <- Back | (W) ^ Up | (X) v Down | (S) - Select")
{}