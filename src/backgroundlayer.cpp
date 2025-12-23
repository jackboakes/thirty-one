#include "backgroundlayer.h"

// Helper fuction to test colour changes with shader
Vector3 BackgroundLayer::ColourToVec3(Color colour)
{
	return Vector3 {
		colour.r / 255.0f,
		colour.g / 255.0f,
		colour.b / 255.0f
	};
}

BackgroundLayer::BackgroundLayer()
{
	m_backgroundShader = LoadShader(0, "../res/shaders/placeholder.fs");
	m_timeLoc = GetShaderLocation(m_backgroundShader, "iTime");
	m_resolutionLoc = GetShaderLocation(m_backgroundShader, "iResolution");
	m_colourLoc = GetShaderLocation(m_backgroundShader, "iColour");
}

BackgroundLayer::~BackgroundLayer()
{

}

void BackgroundLayer::Update(float deltaTime)
{

}

void BackgroundLayer::Draw()
{
	float windowWidth { static_cast<float>(GetScreenWidth()) };
	float windowHeight { static_cast<float>(GetScreenHeight()) };

	// Background shader
	float time { static_cast<float>(GetTime()) };
	float resolution[2] { windowWidth, windowHeight };
	Vector3 colour { ColourToVec3(PURPLE) };
	SetShaderValue(m_backgroundShader, m_timeLoc, &time, SHADER_UNIFORM_FLOAT);
	SetShaderValue(m_backgroundShader, m_resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
	SetShaderValue(m_backgroundShader, m_colourLoc, &colour, SHADER_UNIFORM_VEC3);

	BeginShaderMode(m_backgroundShader);
	DrawRectangle(0, 0, windowWidth, windowHeight, BLACK);
	EndShaderMode();
}