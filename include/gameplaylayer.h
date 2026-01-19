#pragma once
#include "layer.h"
#include "gamestate.h"
#include "handzone.h"
#include "discardzone.h"
#include "deckzone.h"

struct CanvasTransform
{
    float scale;
    Vector2 offset;
};

class GameplayLayer : public Layer {
private:
    Camera2D  m_Camera2D {0};
    GameState m_State;
    DeckZone  m_DeckZone;
    HandZone  m_HandZone;
    DiscardZone m_DiscardZone;

    // Handle User input/events
    void ProcessMessages();
    // Calculate the target state
    void UpdateLogic(float deltaTime);
    // Move the visual state towards target
    void UpdateAnimation(float deltaTime);


    CanvasTransform GetCanvasTransform() const;
    int GetCardIdUnderMouse(Vector2 worldPosition) const;
public:
    GameplayLayer();
    ~GameplayLayer() override;

    void Update(float deltatime) override;
    void Draw() override;

    bool OnMouseButtonPressed() override;
    bool OnMouseButtonReleased() override;
    bool OnMouseMoved(Vector2 position) override { return false; }
    bool OnMouseHover(Vector2 position) override;
};