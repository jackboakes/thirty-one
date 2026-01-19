#pragma once
#include "zone.h"

class DiscardZone : public Zone {
private:
	float m_stackOffset { 0.3f };

public:
	DiscardZone();

	void UpdateLayout(GameState& state) override;
	void Draw() override;
};