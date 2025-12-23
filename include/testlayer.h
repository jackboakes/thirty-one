#pragma once
#include "raylib.h"
#include "layer.h"
#include "globals.h"
#include "card.h"
#include <algorithm>

class TestLayer : public Layer {
private:
	Texture2D m_cardAtlas {};
	RenderTexture2D m_gameCanvas {};
public:
	TestLayer();
	~TestLayer() override;
	void Update(float deltatime) override;
	void Draw() override;
};