#include "application.h"
#include <algorithm>
#include "globals.h"



Application::Application()
{
	// Window
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(GameResolution::width, GameResolution::height, "Thirty One");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);
	SetWindowMinSize(640, 360);
	SetTargetFPS(60);
}

Application::~Application()
{
	for (Layer* layer : m_layerStack)
		delete layer;
	m_layerStack.clear();

	CloseWindow();
}

Application& Application::Instance()
{
	static Application instance;
	return instance;
}

void Application::Run()
{
	while (!WindowShouldClose())
	{
		float deltaTime { GetFrameTime() };
		Update(deltaTime);
		Draw();
	}
}

void Application::Update(float deltaTime)
{
	for (Layer* layer : m_layerStack)
	{
		layer->Update(deltaTime);
	}
}

void Application::Draw()
{
	BeginDrawing();
	for (Layer* layer : m_layerStack)
	{
		layer->Draw();
	}
	EndDrawing();
}

