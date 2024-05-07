#pragma once

enum class Button : short {
	Up,
	Down,
	Left,
	Right,
	SwitchToBigShip,
	SwitchToSmallShip,
	NewGame,
	Levels,
	Info,
	Exit,
	Pause,
	NumOfButtons
};

struct ButtonState {
	const bool Pressed;
	const bool Released;
	const bool Held;
	ButtonState(bool pressed, bool released, bool held);
};




