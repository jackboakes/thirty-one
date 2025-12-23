#pragma once
#include "raylib.h"
#include <vector>
#include "layer.h"



class Application {
private:
	std::vector<Layer*> m_layerStack;

	Application();
	~Application();
	void Update(float deltaTime);
	void Draw();

public:
	static Application& Instance();
	void Run();

	void PushLayer(Layer* state)
	{
		m_layerStack.push_back(state);
	}

	void TransitionLayer(Layer* toState)
	{
		// TODO:: Not sure how to handle this yet
	}
};