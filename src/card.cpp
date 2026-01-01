#include "card.h"
#include "raylib.h"
#include "raymath.h"

Texture2D Card::s_CardAtlasTexture;
int Card::s_RefCount = 0;

Card::Card(Suit suit, Rank rank)
    : m_Suit(suit), m_Rank(rank)
{
    if (s_RefCount == 0)
    {
        s_CardAtlasTexture = LoadTexture("../assets/cardAtlas.png");
        SetTextureFilter(s_CardAtlasTexture, TEXTURE_FILTER_POINT);
    }
    s_RefCount++;

    float spriteWidth = 64.0f;
    float spriteHeight = 96.0f;

    size = { spriteWidth * 2.0f, spriteHeight * 2.0f };
    m_SourceRec = GetSourceRec(spriteWidth, spriteHeight);
}

Card::~Card()
{
    s_RefCount--;
    if (s_RefCount <= 0)
    {
        UnloadTexture(s_CardAtlasTexture);
    }

}

Rectangle Card::GetSourceRec(float slotWidth, float slotHeight) const
{
    int xIndex { 0 };
    int yIndex { 0 };

    switch (m_Suit)
    {
    case Suit::Diamonds: yIndex = 0; break;
    case Suit::Clubs:    yIndex = 1; break; 
    case Suit::Hearts:   yIndex = 2; break;
    case Suit::Spades:   yIndex = 3; break;
    }


    if (isFaceUp)
    {
        xIndex = static_cast<int>(m_Rank) - 1;
    }
    else
    {
        // default card back
        yIndex = 2;
        xIndex = 15;
    }

    return Rectangle {
        xIndex * slotWidth,
        yIndex * slotHeight,
        slotWidth,
        slotHeight
    };
}

void Card::OnUpdate(float deltaTime)
{
    if (Vector2Distance(renderPosition, worldPosition) < 0.5f)
    {
        renderPosition = worldPosition;
    }
    else
    {
        renderPosition = Vector2Lerp(renderPosition, worldPosition, deltaTime * m_SmoothSpeed);
    }
}

void Card::OnRender()
{
    Rectangle destRec {
            renderPosition.x,
            renderPosition.y,
            size.x,
            size.y
    };

    constexpr Vector2 origin { 0.0f, 0.0f };

    DrawTexturePro(
        s_CardAtlasTexture,
        m_SourceRec,
        destRec,
        origin,
        0.0f, 
        WHITE
    );
}

// Bypass lerp and move rendered instantly to worldPosition
void Card::SnapToPosition()
{
    renderPosition = worldPosition;
}