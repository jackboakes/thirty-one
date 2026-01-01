#include "testlayer.h"
#include "globals.h"
#include "card.h"
#include <algorithm>

class SceneRoot : public Element {};

TestLayer::TestLayer()
{
    m_SceneRoot = std::make_unique<SceneRoot>();

    // Create Hand
    auto playerHand { std::make_unique<Hand>() };

    // Position Hand
    float xPosition { (GameResolution::f_Width - playerHand->size.x) * 0.5f };
    float yPosition { GameResolution::f_Height - playerHand->size.y - 40.0f };
    playerHand->localPosition = { xPosition, yPosition };

    m_HandPtr = playerHand.get();

    // Add Cards
    playerHand->AddCard(std::make_unique<Card>(Suit::Spades, Rank::Ace));
    playerHand->AddCard(std::make_unique<Card>(Suit::Spades, Rank::Ten));
    playerHand->AddCard(std::make_unique<Card>(Suit::Hearts, Rank::Jack));
    playerHand->AddCard(std::make_unique<Card>(Suit::Clubs, Rank::King));
    m_HandPtr = playerHand.get();
    m_SceneRoot->AddChild(std::move(playerHand));
}

TestLayer::~TestLayer()
{
    m_SceneRoot.reset();
}

void TestLayer::Update(float deltaTime)
{
    if (m_SceneRoot)
    {
        m_SceneRoot->Layout();
    }

    if (m_DraggedCard && m_DraggedCard->isDragging)
    {
        const Vector2 worldMouse { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };
        const float screenWidth { static_cast<float>(GetScreenWidth()) };
        const float screenHeight { static_cast<float>(GetScreenHeight()) };

        // Clamp to screen edges
        const float targetX { worldMouse.x + m_DragOffset.x };
        const float targetY { worldMouse.y + m_DragOffset.y };
        const Vector2 minWorld { GetScreenToWorld2D({ 0, 0 }, m_Camera2D) };
        const Vector2 maxWorld { GetScreenToWorld2D({ screenWidth, screenHeight }, m_Camera2D) };

        m_DraggedCard->worldPosition.x = std::clamp(targetX, minWorld.x, maxWorld.x - m_DraggedCard->size.x);
        m_DraggedCard->worldPosition.y = std::clamp(targetY, minWorld.y, maxWorld.y - m_DraggedCard->size.y);
    }

    if (m_SceneRoot)
    {
        m_SceneRoot->Update(deltaTime);
    }
}

void TestLayer::Draw()
{
    CanvasTransform canvasTransform { GetCanvasTransform() };
    m_Camera2D.zoom = canvasTransform.scale;
    m_Camera2D.offset = canvasTransform.offset;
    m_Camera2D.target = { 0, 0 };

    BeginMode2D(m_Camera2D);

    if (m_SceneRoot) m_SceneRoot->Render();

    // Render dragged out card on top of hand/other elements
    if (m_DraggedCard && m_DraggedCard->isDragging)
    {
        m_DraggedCard->Render();
    }

    EndMode2D();
}

bool TestLayer::OnMouseButtonPressed(int button)
{
    if (button == MOUSE_BUTTON_LEFT)
    {
        Vector2 worldMouse { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };

        // Ask the hand what we clicked
        Card* hit { m_HandPtr->GetCardAt(worldMouse) };
        if (hit)
        {
            m_DraggedCard = hit;
            m_DraggedCard->isDragging = true;
            m_DraggedCard->isVisible = false;

            m_DragOffset = {
                m_DraggedCard->worldPosition.x - worldMouse.x,
                m_DraggedCard->worldPosition.y - worldMouse.y
            };
            return true;
        }
    }
    return false;
}

bool TestLayer::OnMouseButtonReleased(int button)
{
    if (button == MOUSE_BUTTON_LEFT)
    {
        if (m_DraggedCard != nullptr)
        {
            m_DraggedCard->isVisible = true;
            m_DraggedCard->isDragging = false;
            m_DraggedCard = nullptr;
        }
        return true;
    }
    return false;
}

// Calculates the render transform required to center and scale the game content 
// to the current window dimensions without stretching.
CanvasTransform TestLayer::GetCanvasTransform() const
{
    const float windowWidth { static_cast<float>(GetScreenWidth()) };
    const float windowHeight { static_cast<float>(GetScreenHeight()) };

    const float scale { std::min(
        windowWidth / GameResolution::f_Width,
        windowHeight / GameResolution::f_Height
    ) };

    Vector2 offset {
        (windowWidth - GameResolution::f_Width * scale) * 0.5f,
        (windowHeight - GameResolution::f_Height * scale) * 0.5f
    };

    return { scale, offset };
}


