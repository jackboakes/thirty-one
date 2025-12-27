#pragma once
#include "raylib.h"
#include "layer.h"
#include "element.h"
#include <memory>


class TestLayer : public Layer {
private:
	RenderTexture2D m_gameCanvas {};
	std::unique_ptr<Element> m_SceneRoot;

public:
	TestLayer();
	~TestLayer() override;
	void Update(float deltatime) override;
	void Draw() override;
};