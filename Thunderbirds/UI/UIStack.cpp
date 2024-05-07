#include "UIStack.h"

void UIStack::Update(float delta, const IController& controller) {
	if (_stack.empty()) {
		return;
	}

	_stack.top()->Update(delta, controller);
}

void UIStack::Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
	if (_stack.empty()) {
		return;
	}

	_stack.top()->Draw(screenWidth, screenHight, renderer);
}

void UIStack::Push(UIElement& element) {
	_stack.push(&element);
}

void UIStack::Pop() {
	_stack.pop();
}

bool UIStack::Empty() const {
	return _stack.empty();
}

int UIStack::Size() const {
	return _stack.size();
}