#pragma once

enum class Button : short {
	Up,
	Down,
	Left,
	Right,
	SwitchToBigShip,
	SwitchToSmallShip,
	NewGame,
	Info,
	Exit,
	Pause,
	NumOfButtons
};

typedef struct buttonState {
	const bool Pressed;
	const bool Released;
	const bool Held;
public:
	buttonState(bool pressed, bool released, bool held) :
		Pressed(pressed) , Released(released), Held(held) {}
} ButtonState;

class IController {
public:
	virtual ButtonState GetButtonState(Button btn) const = 0;
};