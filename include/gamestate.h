#pragma once
#include "raylib.h"
#include <vector>
#include <deque>

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

enum class CardZone {
	Hand,
	Deck

	// discard pile
};

struct CardEntity {
	int id;
	Suit suit;
	Rank rank;

	CardZone zone;
	int ownerIndex { 0 }; // replace with enum depending on direction of game
	int sortOrder { 0 };

	// Visual location
	Vector3 position { 0.0f, 0.0f, 0.0f };
	float rotation { 0.0f };
	float scale { 1.0f };
	bool isFaceUp { false };
	bool isVisible { true };

	// Animation target location
	Vector3 targetPosition { 0.0f, 0.0f, 0.0f };
	float targetRotation { 0.0f };
};


// TODO:: I could make this generic and reusable
struct Message {
	enum class Kind {
		None = 0,
		CardHovered,
		CardClicked,
		CardDragStart,
		CardDropped,
		DrawCard,
		// pass turn
	} kind { Kind::None };

	struct Pararms {
		int cardID { -1 };
		int ownerID { -1 };
		CardZone targetZone { CardZone::Hand };
		Vector2 mousePosition { 0.0f, 0.0f };
	} params;
};

struct GameState {
	std::vector<CardEntity> cards;

	// Queue of input and events
	std::deque<Message> messages;

	// store player turn

	struct DragState {
		bool isActive { false };
		int cardId = -1;
		Vector2 dragOffset { 0.0f, 0.0f };
	} drag;

	int hoveredCardId = -1;
};

