#pragma once
#include "raylib.h"
#include "element.h"

enum class Suit {
	Diamonds,
	Clubs,
	Hearts,
	Spades
};

enum class Rank {
	Ace = 1,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King
};

class Card : public Element {
private:
	Suit m_Suit {};
	Rank m_Rank {};
	static Texture2D s_CardAtlasTexture;
	static int s_RefCount;
	Rectangle m_SourceRec {};
	float m_SmoothSpeed { 10.0f };

protected:
	void OnUpdate(float deltaTime) override;
	void OnRender() override;

public:
	Card(Suit suit, Rank rank);
	~Card();
	Vector2 renderPosition { 0.0f, 0.0f };
	bool isFaceUp { true };
	Vector2 dragScreenPosition;
	bool isDragging { false };

	Rectangle GetSourceRec(float slotWidth, float slotHeight) const;

	void SnapToPosition();
};