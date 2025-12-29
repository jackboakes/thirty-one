#pragma once
#include "raylib.h"

class Layer {
private:

public:
	virtual ~Layer() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	virtual bool OnMouseButtonPressed(int button) { return false; }
	virtual bool OnMouseButtonReleased(int button) { return false; }
	virtual bool OnMouseMoved(Vector2 position) { return false; }
};