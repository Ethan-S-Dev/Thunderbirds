#pragma once
#include "../Core/IController.h"
#include "../Rendering/IRenderer.h"

class UIElement {
public:
	virtual void Update(float delta, const IController& controller) = 0;
	virtual void Draw(float delta, IRenderer& renderer) const = 0;
};