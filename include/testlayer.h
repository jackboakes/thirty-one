#pragma once
#include "layer.h"
#include "element.h"
#include "hand.h"
#include <memory>

class TestLayer : public Layer {
private:
    RenderTexture2D m_gameCanvas {};
    std::unique_ptr<Element> m_SceneRoot;

    Hand* m_HandPtr { nullptr };
    Element* m_DraggedCard { nullptr };
    Vector2 m_DragOffset { 0.0f, 0.0f };

    // Helper to fix mouse coordinates
    Vector2 GetCanvasMousePosition() const;

public:
    TestLayer();
    ~TestLayer() override;
    void Update(float deltatime) override;
    void Draw() override;

    bool OnMouseButtonPressed(int button) override;
    bool OnMouseButtonReleased(int button) override;
    bool OnMouseMoved(Vector2 position) override { return false; }
};