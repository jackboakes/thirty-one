#pragma once

struct GameResolution
{
	static constexpr int width { 1920 };
	static constexpr int height { 1080 };
	static constexpr float f_Width { static_cast<float>(width) };
	static constexpr float f_Height { static_cast<float>(height) };
};