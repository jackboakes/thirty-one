#pragma once
#include "layer.h"
#include "gamestate.h"
#include "handzone.h"

struct CanvasTransform
{
    float scale;
    Vector2 offset;
};

class TestLayer : public Layer {
private:
    Camera2D  m_Camera2D {0};
    GameState m_State;
    HandZone  m_HandZone;


    CanvasTransform GetCanvasTransform() const;
    int GetCardIdUnderMouse(Vector2 worldPosition) const;
public:
    TestLayer();
    ~TestLayer() override;

    void Update(float deltatime) override;
    void Draw() override;

    bool OnMouseButtonPressed() override;
    bool OnMouseButtonReleased() override;
    bool OnMouseMoved(Vector2 position) override { return false; }
    bool OnMouseHover(Vector2 position) override;
};