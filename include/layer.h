#pragma once
#include "raylib.h"

class Layer {
private:

public:
	virtual ~Layer() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	virtual bool OnMouseButtonPressed() { return false; }
	virtual bool OnMouseButtonReleased() { return false; }
	virtual bool OnMouseMoved(Vector2 position) { return false; }
	virtual bool OnMouseHover(Vector2 position) { return false; }
};