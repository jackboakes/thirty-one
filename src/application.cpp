#include "application.h"

Application::Application()
{
	// Window
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	// Smaller than screen size so that the title bar doesn't get hidden
	InitWindow(1280, 720, "Thirty One");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);
	SetWindowMinSize(640, 360);
	SetTargetFPS(60);
}

Application::~Application()
{
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
		ProcessInput();
		float deltaTime { GetFrameTime() };
		Update(deltaTime);
		Draw();
	}
}

void Application::ProcessInput()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
	{
		for (auto it { m_layerStack.rbegin() }; it != m_layerStack.rend(); ++it) 
		{
			if ((*it)->OnMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				break;
			}
		}
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
	{
		for (auto it { m_layerStack.rbegin() }; it != m_layerStack.rend(); ++it) 
		{
			if ((*it)->OnMouseButtonReleased(MOUSE_BUTTON_LEFT))
			{
				break;
			}
		}
	}

	Vector2 mouseDelta = GetMouseDelta();
	if (mouseDelta.x != 0 || mouseDelta.y != 0) 
	{
		for (auto it { m_layerStack.rbegin() }; it != m_layerStack.rend(); ++it)
		{
			Vector2 mousePosition = GetMousePosition();
			if ((*it)->OnMouseMoved(mousePosition))
			{
				break;
			}
		}
	}
}

void Application::Update(float deltaTime)
{
	for (const std::unique_ptr<Layer>& layer : m_layerStack)
	{
		layer->Update(deltaTime);
	}
}

void Application::Draw()
{
	BeginDrawing();
	for (const std::unique_ptr<Layer>& layer : m_layerStack)
	{
		layer->Draw();
	}
	EndDrawing();
}

