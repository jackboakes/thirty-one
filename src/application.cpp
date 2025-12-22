#include "application.h"
#include <algorithm>

// Helper fuction to test colour changes with shader
Vector3 ColourToVec3(Color colour)
{
	return Vector3 {
		colour.r / 255.0f,
		colour.g / 255.0f,
		colour.b / 255.0f
	};
}

Application::Application()
{
	// Window
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(GameResolution::width, GameResolution::height, "Thirty One");
	SetWindowState(FLAG_WINDOW_MAXIMIZED);
	SetWindowMinSize(640, 360);
	SetTargetFPS(60);


	m_backgroundShader = LoadShader(0, "../res/shaders/placeholder.fs");
	m_TimeLoc = GetShaderLocation(m_backgroundShader, "iTime");
	m_ResolutionLoc = GetShaderLocation(m_backgroundShader, "iResolution");
	m_ColourLoc = GetShaderLocation(m_backgroundShader, "iColour");

	// Virtual screen for game area
	m_gameCanvas = LoadRenderTexture(GameResolution::width, GameResolution::height);
	SetTextureFilter(m_gameCanvas.texture, TEXTURE_FILTER_BILINEAR);
}

Application::~Application()
{
	UnloadRenderTexture(m_gameCanvas);
	CloseWindow();
}

void Application::Run()
{
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}
}

void Application::Update()
{

}

void Application::Draw()
{
	BeginTextureMode(m_gameCanvas);
	ClearBackground(BLANK);

	// turn this on to see where the texture2d is rendering
	DrawRectangle(0, 0, GameResolution::width, GameResolution::height, Fade(BLACK, 0.5f));
	DrawCircle(640, 360, 50, RED);

	EndTextureMode();

	BeginDrawing();
	ClearBackground(BLACK);

	float windowWidth { static_cast<float>(GetScreenWidth()) };
	float windowHeight { static_cast<float>(GetScreenHeight()) };

	// Background shader
	float time { static_cast<float>(GetTime()) };
	float resolution[2] { windowWidth, windowHeight };
	Vector3 colour { ColourToVec3(PURPLE) };
	SetShaderValue(m_backgroundShader, m_TimeLoc, &time, SHADER_UNIFORM_FLOAT);
	SetShaderValue(m_backgroundShader, m_ResolutionLoc, resolution, SHADER_UNIFORM_VEC2);
	SetShaderValue(m_backgroundShader, m_ColourLoc, &colour, SHADER_UNIFORM_VEC3);

	BeginShaderMode(m_backgroundShader);
	DrawRectangle(0, 0, windowWidth, windowHeight, BLACK);
	EndShaderMode();


	float scale { std::min(windowWidth / GameResolution::f_Width, windowHeight / GameResolution::f_Height) };
	float scaledWidth { GameResolution::f_Width * scale };
	float scaledHeight { GameResolution::f_Height * scale };
	float offsetX { (windowWidth - scaledWidth) * 0.5f };
	float offsetY { (windowHeight - scaledHeight) * 0.5f };

	// Overlay the game canvas
	DrawTexturePro(
		m_gameCanvas.texture,
		{ 0, 0, GameResolution::f_Width, -GameResolution::f_Height },
		{ offsetX, offsetY, scaledWidth, scaledHeight },
		{ 0, 0 },
		0.0f,
		WHITE
	);

	EndDrawing();
}