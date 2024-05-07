#pragma once
#include <stack>
#include "UIElement.h"

class UIStack : public UIElement {
private:
	std::stack<UIElement*> _stack;
public:
	void Update(float delta, const IController& controller);
	void Draw(int screenWidth, int screenHight, IRenderer& renderer) const;
	void Push(UIElement& element);
	void Pop();
	bool Empty() const;
	int Size() const;
};