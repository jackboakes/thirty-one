#pragma once
#include "raylib.h"
#include "gamestate.h"

class Zone {
protected:
	Rectangle m_bounds {};

public:
	virtual ~Zone() = default;

	virtual void UpdateLayout(GameState& state) = 0;
	virtual void Draw() = 0;

	// Whether this zone accepts card drops (deck does not)
	virtual bool AcceptsDrops() const { return true; }

	bool IsPointInside(Vector2 worldPosition) const {
		return CheckCollisionPointRec(worldPosition, m_bounds);
	}

	Rectangle GetBounds() const { return m_bounds; }
};
