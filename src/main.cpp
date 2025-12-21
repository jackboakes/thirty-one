#include "raylib.h"
#include <algorithm>

struct GameResolution
{
    static constexpr int width { 1280 };
    static constexpr int height { 720 };
    static constexpr float f_Width { static_cast<float>(width) };
    static constexpr float f_Height { static_cast<float>(height) };
};


// Helper fuction to test colour changes with shader
Vector3 ColourToVec3(Color colour)
{
    return Vector3 {
        colour.r / 255.0f,
        colour.g / 255.0f,
        colour.b / 255.0f
    };
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(GameResolution::width, GameResolution::height, "Thirty One");
    
    Shader shader = LoadShader(0, "../res/shaders/placeholder.fs");

    int iTimeLoc { GetShaderLocation(shader, "iTime") };
    int iResolutionLoc { GetShaderLocation(shader, "iResolution") };
    int iColourLoc { GetShaderLocation(shader, "iColour") };

    // start with window max size
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetWindowMinSize(640, 360);

    RenderTexture2D gameTarget { LoadRenderTexture(GameResolution::width, GameResolution::height) };
    SetTextureFilter(gameTarget.texture, TEXTURE_FILTER_BILINEAR);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float windowWidth { static_cast<float>(GetScreenWidth()) };
        float windowHeight { static_cast<float>(GetScreenHeight()) };

        float time { static_cast<float>(GetTime()) };
        float resolution[2] { windowWidth, windowHeight };
        Vector3 fireColor { ColourToVec3(PURPLE) };
        SetShaderValue(shader, iTimeLoc, &time, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, iResolutionLoc, resolution, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, iColourLoc, &fireColor, SHADER_UNIFORM_VEC3);

        float scale { std::min(windowWidth / GameResolution::f_Width, windowHeight / GameResolution::f_Height) };

        float scaledWidth { GameResolution::f_Width * scale };
        float scaledHeight { GameResolution::f_Height * scale };

        float offsetX { (windowWidth - scaledWidth) * 0.5f };
        float offsetY { (windowHeight - scaledHeight) * 0.5f };

        // draw gameplay area to RenderTexture2D
        BeginTextureMode(gameTarget);
        ClearBackground(BLANK);

        // turn this on to see where the texture2d is rendering
        DrawRectangle(0, 0, GameResolution::width, GameResolution::height, Fade(BLACK, 0.5f));

        DrawCircle(640, 360, 50, RED);


        EndTextureMode();

        BeginDrawing();

        // letterbox area background
        ClearBackground(BLACK);

        BeginShaderMode(shader);
        DrawRectangle(0, 0, windowWidth, windowHeight, BLACK);
        EndShaderMode();

        // Draw Gameplay Area
        DrawTexturePro(
            gameTarget.texture,
            { 0, 0, GameResolution::f_Width, -GameResolution::f_Height },
            { offsetX, offsetY, scaledWidth, scaledHeight },
            { 0, 0 },
            0.0f,
            WHITE
        );

        EndDrawing();
    }

    UnloadRenderTexture(gameTarget);
    CloseWindow();
}