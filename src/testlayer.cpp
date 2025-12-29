#include "testlayer.h"
#include "globals.h"
#include "card.h"
#include <algorithm>

class SceneRoot : public Element {
protected:
	void OnRender() override {}
	void OnLayout() override {}
};

TestLayer::TestLayer()
{
    m_gameCanvas = LoadRenderTexture(GameResolution::width, GameResolution::height);
    SetTextureFilter(m_gameCanvas.texture, TEXTURE_FILTER_BILINEAR);

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
    UnloadRenderTexture(m_gameCanvas);
    m_SceneRoot.reset();
}

Vector2 TestLayer::GetCanvasMousePosition() const
{
    const float windowWidth { static_cast<float>(GetScreenWidth()) };
    const float windowHeight { static_cast<float>(GetScreenHeight()) };

    const float scale { std::min((windowWidth / GameResolution::f_Width), (windowHeight / GameResolution::f_Height)) };
    const float offsetX { (windowWidth - (GameResolution::f_Width * scale)) * 0.5f };
    const float offsetY { (windowHeight - (GameResolution::f_Height * scale)) * 0.5f };

    const Vector2 mouse { GetMousePosition() };
    return { ((mouse.x - offsetX) / scale), ((mouse.y - offsetY) / scale) };
}

void TestLayer::Update(float deltaTime)
{
    if (m_SceneRoot)
    {
        m_SceneRoot->Layout();
    }

    if (m_DraggedCard)
    {
        const Vector2 mousePosition { GetCanvasMousePosition() };
        // set dragged card to these positions if inside of clamp range
        const float targetX { mousePosition.x + m_DragOffset.x };
        const float targetY { mousePosition.y + m_DragOffset.y };

        const float maxX { GameResolution::f_Width - m_DraggedCard->size.x };
        const float maxY { GameResolution::f_Height - m_DraggedCard->size.y };

        // clamp card to edge of screen if mouse goes out of bounds
        m_DraggedCard->screenPosition.x = std::clamp(targetX, 0.0f, maxX);
        m_DraggedCard->screenPosition.y = std::clamp(targetY, 0.0f, maxY);
    }
}

void TestLayer::Draw()
{
    BeginTextureMode(m_gameCanvas);
    ClearBackground(BLANK);

    if (m_SceneRoot) m_SceneRoot->Render();

    if (m_DraggedCard) m_DraggedCard->Render();

    EndTextureMode();

    const float windowWidth { static_cast<float>(GetScreenWidth()) };
    const float windowHeight { static_cast<float>(GetScreenHeight()) };
    const float scale { std::min((windowWidth / GameResolution::f_Width), (windowHeight / GameResolution::f_Height)) };
    float scaledWidth { GameResolution::f_Width * scale };
    float scaledHeight { GameResolution::f_Height * scale };
    float offsetX { (windowWidth - scaledWidth) * 0.5f };
    float offsetY { (windowHeight - scaledHeight) * 0.5f };

    DrawTexturePro(m_gameCanvas.texture,
        { 0, 0, GameResolution::f_Width, -GameResolution::f_Height },
        { offsetX, offsetY, scaledWidth, scaledHeight },
        { 0, 0 }, 0.0f, WHITE);
}

bool TestLayer::OnMouseButtonPressed(int button)
{
    if (button == MOUSE_BUTTON_LEFT)
    {
        const Vector2 mousePosition { GetCanvasMousePosition() };

        // Ask the hand what we clicked
        Element* hit { m_HandPtr->GetCardAt(mousePosition) };
        if (hit)
        {
            m_DraggedCard = hit;
            // Calculate offset so center doesn't snap to mouse
            m_DragOffset = { hit->screenPosition.x - mousePosition.x, hit->screenPosition.y - mousePosition.y };
            return true;
        }
    }
    return false;
}

bool TestLayer::OnMouseButtonReleased(int button)
{
    if (button == MOUSE_BUTTON_LEFT)
    {
        m_DraggedCard = nullptr;
        return true;
    }
    return false;
}