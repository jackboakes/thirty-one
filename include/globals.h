#pragma once
#include "raylib.h"

struct GameResolution
{
	static constexpr int width { 1280 };
	static constexpr int height { 720 };
	static constexpr float f_Width { static_cast<float>(width) };
	static constexpr float f_Height { static_cast<float>(height) };
};