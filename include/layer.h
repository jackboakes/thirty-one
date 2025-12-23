#pragma once

class Layer {
private:

public:
	virtual ~Layer() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;
};