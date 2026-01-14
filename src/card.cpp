#include "card.h"
#include "raylib.h"
#include "algorithm"

Texture2D CardRenderer::s_Atlas;
Shader CardRenderer:: s_PixelShader;

void CardRenderer::Initialise()
{
    if (s_Atlas.id == 0)
    {
        s_Atlas = LoadTexture("../assets/cardAtlas.png");
        SetTextureFilter(s_Atlas, TEXTURE_FILTER_BILINEAR);
    }

    if (s_PixelShader.id == 0)
    {
        s_PixelShader = LoadShader(0, "../res/shaders/pixelfiltering.fs");
    }

    int sizeLocation = GetShaderLocation(s_PixelShader, "textureSize");
    float size[2] = { static_cast<float>(s_Atlas.width), static_cast<float>(s_Atlas.height) };
    SetShaderValue(s_PixelShader, sizeLocation, size, SHADER_UNIFORM_VEC2);
}

void CardRenderer::Shutdown()
{
    if (s_Atlas.id != 0)
    {
        UnloadTexture(s_Atlas);
        s_Atlas.id = 0;
    }
    
    if (s_PixelShader.id != 0)
    {
        UnloadShader(s_PixelShader);
        s_PixelShader.id = 0;
    }
}

Vector2 CardRenderer::GetSize()
{
    // size of cards sprites are 64x96 source * 2 scale
    return { 128.0f, 192.0f };
}

Rectangle CardRenderer::GetSourceRectangle(Suit suit, Rank rank, bool isFaceUp) 
{
    float slotWidth { 64.0f };
    float slotHeight { 96.0f };
    int xIndex { 0 };
    int yIndex { 0 };

    if (isFaceUp)
    {
        switch (suit)
        {
        case Suit::Diamonds: yIndex = 0; break;
        case Suit::Clubs:    yIndex = 1; break;
        case Suit::Hearts:   yIndex = 2; break;
        case Suit::Spades:   yIndex = 3; break;
        }

        xIndex = static_cast<int>(rank) - 1;
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

void CardRenderer::Draw(const CardEntity& card)
{
    if (!card.isVisible)
    {
        return;
    }

    const Rectangle source { GetSourceRectangle(card.suit, card.rank, card.isFaceUp) };
    
    Vector2 originalSize { GetSize() };
    float centerX = card.position.x + (originalSize.x * 0.5f);
    float centerY = card.position.y + (originalSize.y * 0.5f);

    const Rectangle destination {
            centerX,
            centerY,
            originalSize.x * card.scale,
            originalSize.y * card.scale
    };

    const Vector2 origin {
        destination.width * 0.5f,
        destination.height * 0.5f
    };

    // Animate smoothly the shadow as scale increases
    const float rawHeight = (card.scale - 0.975f) * 200.0f;
    // Clamp to max shadow to 20 pixels
    float height = std::min(rawHeight, 20.0f);
    const float baseDist = 8.0f;

    // Light source is at centre of screen
    const Vector2 screenCenter = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    Vector2 distFromCenter = { centerX - screenCenter.x, centerY - screenCenter.y };

    // Negate so the shadow pulls into the centre of the screen
    Vector2 shadowOffset = {
        distFromCenter.x * -0.02f,
        distFromCenter.y * -0.02f
    };

    // Determine the side the shadow is on
    shadowOffset.x += (shadowOffset.x > 0 ? height : -height) * 0.05f;
    shadowOffset.y += baseDist + height;

    Rectangle shadowDest = destination;
    shadowDest.x += shadowOffset.x;
    shadowDest.y += shadowOffset.y;

    // Shadow
    DrawTexturePro(
        s_Atlas,
        source,
        shadowDest,
        origin,
        card.rotation,
        Fade(BLACK, 0.25f)
    );

    BeginShaderMode(s_PixelShader);

    // Regular card
    DrawTexturePro(
        s_Atlas,
        source,
        destination,
        origin,
        card.rotation, 
        WHITE
    );

    EndShaderMode();
}
