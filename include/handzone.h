#include "raylib.h"
#include "gamestate.h"

class HandZone {
private:
	// Max size we have can have hand at any point in game
	const int m_capacity { 4 };
	// Horizontal padding 
	const float m_padding { 20.0f };
	// The % of a card showing without overlap
	const float m_cardSpacing { 0.75f };

	// The visual area of the hand container/zone
	Rectangle m_bounds {};

public:
	HandZone();

	void UpdateLayout(GameState& state);
	// Draw the hand container only
	void Draw();
	bool IsPointInside(Vector2 worldPosition) const;
};