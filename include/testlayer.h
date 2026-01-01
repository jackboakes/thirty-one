#pragma once
#include "layer.h"
#include "element.h"
#include "hand.h"
#include "card.h"
#include <memory>

struct CanvasTransform
{
    float scale;
    Vector2 offset;
};

class TestLayer : public Layer {
private:
    Camera2D  m_Camera2D {0};
    std::unique_ptr<Element> m_SceneRoot;

    Hand* m_HandPtr { nullptr };
    Card* m_DraggedCard { nullptr };
    Vector2 m_DragOffset { 0.0f, 0.0f };

    CanvasTransform GetCanvasTransform() const;

public:
    TestLayer();
    ~TestLayer() override;
    void Update(float deltatime) override;
    void Draw() override;

    bool OnMouseButtonPressed(int button) override;
    bool OnMouseButtonReleased(int button) override;
    bool OnMouseMoved(Vector2 position) override { return false; }
};