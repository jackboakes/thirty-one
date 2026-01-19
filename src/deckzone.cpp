#include "deckzone.h"
#include "card.h"
#include "globals.h"
#include <vector>
#include <algorithm>

DeckZone::DeckZone()
{
	const Vector2 cardSize { CardRenderer::GetSize() };

	// Position deck in centre-left of screen
	const float centreX { (GameResolution::f_Width * 0.5f) - cardSize.x - 40.0f };
	const float centreY { (GameResolution::f_Height * 0.5f) - (cardSize.y * 0.5f) };

	m_bounds = {
		centreX,
		centreY,
		cardSize.x,
		cardSize.y
	};
}

void DeckZone::UpdateLayout(GameState& state)
{
	std::vector<int> deckIds;

	for (int i { 0 }; i < state.cards.size(); i++)
	{
		if (state.cards[i].zone == CardZone::Deck)
		{
			deckIds.push_back(i);
		}
	}

	if (deckIds.empty())
	{
		return;
	}

	// Sort by sortOrder to maintain deck order
	std::sort(deckIds.begin(), deckIds.end(), [&](int a, int b) {
		return state.cards[a].sortOrder < state.cards[b].sortOrder;
	});

	for (int i { 0 }; i < deckIds.size(); i++)
	{
		int id { deckIds[i] };
		float offset { i * m_stackOffset };

		state.cards[id].targetPosition = {
			m_bounds.x + offset,
			m_bounds.y - offset,
			0
		};

		state.cards[id].sortOrder = i;
		state.cards[id].isFaceUp = false;
	}
}

void DeckZone::Draw()
{
	DrawRectangleRounded(m_bounds, 0.1f, 10, Fade(BLACK, 0.35f));
}
