#pragma once
#include <stack>
#include "UIElement.h"

class UIStack : public UIElement {
private:
	std::stack<UIElement*> _stack;
public:
	void Update(float delta, const IController& controller) {
		if (_stack.empty()) {
			return;
		}

		_stack.top()->Update(delta, controller);
	}
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const {
		if (_stack.empty()) {
			return;
		}

		_stack.top()->Draw(screenWidth, screenHight, renderer);
	}
	void Push(UIElement& element) {
		_stack.push(&element);
	}
	void Pop() {
		_stack.pop();
	}
	bool Empty() const {
		return _stack.empty();
	}
	int Size() const {
		return _stack.size();
	}
};