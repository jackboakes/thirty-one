#include "testlayer.h"
#include "raymath.h"
#include "globals.h"
#include "card.h"
#include <algorithm>


TestLayer::TestLayer()
{
    CardRenderer::Initialise();

    CardEntity aceOfSpades;
    aceOfSpades.id = 0;
    aceOfSpades.rank = Rank::Ace;
    aceOfSpades.suit = Suit::Spades;
    aceOfSpades.zone = CardZone::Hand;
    aceOfSpades.isFaceUp = true;
    m_State.cards.push_back(aceOfSpades);

    CardEntity tenOfSpades;
    tenOfSpades.id = 1;
    tenOfSpades.rank = Rank::Ten;
    tenOfSpades.suit = Suit::Spades;
    tenOfSpades.zone = CardZone::Hand;
    tenOfSpades.isFaceUp = true;
    m_State.cards.push_back(tenOfSpades);

    CardEntity jackOfHearts;
    jackOfHearts.id = 2;
    jackOfHearts.rank = Rank::Jack;
    jackOfHearts.suit = Suit::Hearts;
    jackOfHearts.zone = CardZone::Hand;
    jackOfHearts.isFaceUp = true;
    m_State.cards.push_back(jackOfHearts);

    CardEntity kingOfClubs;
    kingOfClubs.id = 3;
    kingOfClubs.rank = Rank::King;
    kingOfClubs.suit = Suit::Clubs;
    kingOfClubs.zone = CardZone::Hand;
    kingOfClubs.isFaceUp = true;
    m_State.cards.push_back(kingOfClubs);

    for (auto& card : m_State.cards) 
    {
        card.position = { GameResolution::f_Width / 2, GameResolution::f_Height / 2, 0 };
    }
}

TestLayer::~TestLayer()
{
    CardRenderer::Shutdown();
}

void TestLayer::Update(float deltaTime)
{
    // process messages
    while (!m_State.messages.empty())
    {
        Message messsage { m_State.messages.front() };
        m_State.messages.pop_front();

        switch (messsage.kind)
        {
        case Message::Kind::CardHovered:
        {
            m_State.hoveredCardId = messsage.params.cardID;
        } break;
        case Message::Kind::CardClicked:
        {
            // Calculate the offset based on where we clicked
            CardEntity& card { m_State.cards[messsage.params.cardID] };
 
            m_State.drag.isActive = true;
            m_State.drag.cardId = messsage.params.cardID;
            m_State.drag.dragOffset = {
                card.position.x - messsage.params.mousePosition.x,
                card.position.y - messsage.params.mousePosition.y
            };

            // Bring card to front
            m_State.cards[messsage.params.cardID].sortOrder = 999;

        } break;
        case Message::Kind::CardDropped:
        {
            if (m_HandZone.IsPointInside(messsage.params.mousePosition))
            {
                int id { messsage.params.cardID };

                m_State.cards[id].zone = CardZone::Hand;
                m_State.cards[id].isFaceUp = true;
            }

            // Drag is done
            m_State.drag.isActive = false;
            m_State.drag.cardId = -1;
        } break;

        default: break;
        }
    }

    // drag physics
    if (m_State.drag.isActive) 
    {
        const Vector2 mouseWorld { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };
        CardEntity& draggedCard { m_State.cards[m_State.drag.cardId] };

        // Calculate Unclamped Target
        const float targetX { mouseWorld.x + m_State.drag.dragOffset.x };
        const float targetY { mouseWorld.y + m_State.drag.dragOffset.y };

        const float screenWidth { static_cast<float>(GetScreenWidth()) };
        const float screenHeight { static_cast<float>(GetScreenHeight()) };

        // Calculate Screen Bounds in World Space
        const Vector2 minWorld { GetScreenToWorld2D({ 0, 0 }, m_Camera2D) };
        const Vector2 maxWorld { GetScreenToWorld2D({ screenWidth, screenHeight }, m_Camera2D) };

        // Clamp the Top-Left corner so that the card stays within bounds
        const Vector2 cardSize { CardRenderer::GetSize() };

        // We divide by 2 because it expands equally in both directions (Center Pivot)
        const float widthPadding { (cardSize.x * draggedCard.scale - cardSize.x) * 0.5f };
        const float heightPadding { (cardSize.y * draggedCard.scale - cardSize.y) * 0.5f };

        const float minX { minWorld.x + widthPadding };
        const float maxX { maxWorld.x - cardSize.x - widthPadding };

        const float minY { minWorld.y + heightPadding };
        const float maxY { maxWorld.y - cardSize.y - heightPadding };

        draggedCard.position.x = std::clamp(targetX, minX, maxX);
        draggedCard.position.y = std::clamp(targetY, minY, maxY);
    }

    m_HandZone.UpdateLayout(m_State);


    // rough animation and drag
    Vector2 mouseWorld { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };

    float snapSpeed { 15.0f };
    float lerpFactor { 1.0f - powf(2.0f, -snapSpeed * deltaTime) };

    for (auto& card : m_State.cards)
    {
        float targetScale { 1.0f };

        if (m_State.drag.isActive && m_State.drag.cardId == card.id) 
        {
            targetScale = 1.1f;
        }
        else if (!m_State.drag.isActive && m_State.hoveredCardId == card.id) 
        {
            targetScale = 1.1f;
        }

        card.scale = Lerp(card.scale, targetScale, lerpFactor);

        if (!(m_State.drag.isActive && m_State.drag.cardId == card.id))
        {
            card.position.x = Lerp(card.position.x, card.targetPosition.x, lerpFactor);
            card.position.y = Lerp(card.position.y, card.targetPosition.y, lerpFactor);
        }
    }

}

void TestLayer::Draw()
{
    CanvasTransform canvasTransform { GetCanvasTransform() };
    m_Camera2D.zoom = canvasTransform.scale;
    m_Camera2D.offset = canvasTransform.offset;
    m_Camera2D.target = { 0, 0 };

    BeginMode2D(m_Camera2D);
    m_HandZone.Draw();

    // Populate with cards
    std::vector<int> drawOrder (m_State.cards.size());
    for (int i { 0 }; i < m_State.cards.size(); i++)
    {
        drawOrder.push_back(i);
    }
        
    // Sort based on draw order
    std::sort(drawOrder.begin(), drawOrder.end(), [&](int a, int b) {
        return m_State.cards[a].sortOrder < m_State.cards[b].sortOrder;
        });

    // Draw
    for (int id : drawOrder) 
    {
        CardRenderer::Draw(m_State.cards[id]);
    }

    EndMode2D();
}

bool TestLayer::OnMouseButtonPressed()
{
    Vector2 mouseWorld { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };
    int hitId { GetCardIdUnderMouse(mouseWorld) };

    if (hitId != -1)
    {
        Message message;
        message.kind = Message::Kind::CardClicked;
        message.params.cardID = hitId;
        message.params.mousePosition = mouseWorld;
        m_State.messages.push_back(message);

        return true;
    }

    return false;
}

bool TestLayer::OnMouseButtonReleased()
{
    if (m_State.drag.isActive)
    {
        Vector2 mouseWorld { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };

        Message message;
        message.kind = Message::Kind::CardDropped;
        message.params.cardID = m_State.drag.cardId;
        message.params.mousePosition = mouseWorld;
        m_State.messages.push_back(message);

        return true;
    }

    return false;
}

bool TestLayer::OnMouseHover(Vector2 positon)
{
    Vector2 mouseWorld { GetScreenToWorld2D(GetMousePosition(), m_Camera2D) };
    int hitId { GetCardIdUnderMouse(mouseWorld) };

    if (hitId != m_State.hoveredCardId)
    {
        Message message;
        message.kind = Message::Kind::CardHovered;
        message.params.cardID = hitId; 
        m_State.messages.push_back(message);

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


int TestLayer::GetCardIdUnderMouse(Vector2 worldPosition) const
{
    for (int i { static_cast<int>(m_State.cards.size() - 1) }; i >= 0; i--)
    {
        const CardEntity& card { m_State.cards[i] };
        const Vector2 size { CardRenderer::GetSize() };
        const Rectangle rectangle { card.position.x, card.position.y, size.x, size.y };

        if (CheckCollisionPointRec(worldPosition, rectangle)) 
        {
            return card.id;
        }
    }

    return -1;
}
