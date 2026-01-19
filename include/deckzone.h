#pragma once
#include "zone.h"

class DeckZone : public Zone {
private:
	// Offset per card to create stacked look
	float m_stackOffset { 0.2f };

public:
	DeckZone();

	void UpdateLayout(GameState& state) override;
	void Draw() override;

	// Deck cannot receive dropped cards - only drawn from
	bool AcceptsDrops() const override { return false; }
};
