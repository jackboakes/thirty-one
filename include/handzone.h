#pragma once
#include "zone.h"

class HandZone : public Zone {
private:
	// Max size we have can have hand at any point in game
	const int m_capacity { 4 };
	// Horizontal padding 
	const float m_padding { 20.0f };
	// The % of a card showing without overlap
	const float m_cardSpacing { 0.75f };

public:
	HandZone();

	void UpdateLayout(GameState& state) override;
	void Draw() override;
};