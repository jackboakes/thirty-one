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

protected:
	void OnRender() override;
	void OnLayout() override {}
public:
	Card(Suit suit, Rank rank);
	~Card();

	bool isFaceUp { true };

	Rectangle GetSourceRec(float slotWidth, float slotHeight) const;
};