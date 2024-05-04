#pragma once

typedef struct buttonState {
	const bool Pressed;
	const bool Released;
	const bool Held;
public:
	buttonState(bool pressed, bool released, bool held) :
		Pressed(pressed), Released(released), Held(held) {}
} ButtonState;

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


