#pragma once
#include "Buttons.h"

class IController {
public:
	virtual ButtonState GetButtonState(Button btn) const = 0;
};