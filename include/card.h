#pragma once
#include "raylib.h"
#include "gamestate.h"

// TODO:: decide wether this could just be a namespace instead 
class CardRenderer {
private:
	static Texture2D s_Atlas;
	static Rectangle GetSourceRectangle(Suit suit, Rank rank, bool isFaceUp);
public:
	static void Initialise();
	static void Shutdown();

	static void Draw(const CardEntity& card);

	static Vector2 GetSize();

};