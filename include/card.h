#pragma once
#include "raylib.h"

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

class Card {
private:

public:
	Suit suit {};
	Rank rank {};
	bool isFaceUp { true };

	Rectangle GetSourceRec(float slotWidth, float slotHeight) const;
};