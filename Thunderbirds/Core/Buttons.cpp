#include "Buttons.h"

ButtonState::ButtonState(bool pressed, bool released, bool held) :
	Pressed(pressed),
	Released(released),
	Held(held) 
{}