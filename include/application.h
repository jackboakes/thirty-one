#pragma once
#include "raylib.h"


struct GameResolution
{
	static constexpr int width { 1280 };
	static constexpr int height { 720 };
	static constexpr float f_Width { static_cast<float>(width) };
	static constexpr float f_Height { static_cast<float>(height) };
};

class Application {
private:
	RenderTexture2D m_gameCanvas {};
	Shader m_backgroundShader {};
	int m_TimeLoc {};
	int m_ResolutionLoc {};
	int m_ColourLoc {};

	Application();
	~Application();
	void Update();
	void Draw();

public:
	static Application& Instance();
	void Run();

	Texture2D cardAtlas;
};