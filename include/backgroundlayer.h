#pragma once
#include "raylib.h"
#include "layer.h"

class BackgroundLayer : public Layer {
private:
	Shader m_backgroundShader {};
	int m_timeLoc {};
	int m_resolutionLoc {};
	int m_colourLoc {};

	Vector3 ColourToVec3(Color colour);
public:
	BackgroundLayer();
	~BackgroundLayer() override;
	void Update(float deltaTime) override;
	void Draw() override;
};