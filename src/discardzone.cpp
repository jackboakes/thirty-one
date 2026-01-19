#include "discardzone.h"
#include "card.h"
#include "globals.h"
#include <vector>
#include <algorithm>


DiscardZone::DiscardZone()
{
	const Vector2 cardSize { CardRenderer::GetSize() };

	const float centreX { (GameResolution::f_Width * 0.5f) + cardSize.x + 40.f};
	const float centreY { (GameResolution::f_Height * 0.5f) - (cardSize.y * 0.5f) };

	m_bounds = {
		centreX,
		centreY,
		cardSize.x,
		cardSize.y
	};
}

void DiscardZone::UpdateLayout(GameState& state)
{
	std::vector<int> discardIds;

	for (int i { 0 }; i < state.cards.size(); i++)
	{
		if (state.cards[i].zone == CardZone::Discard)
		{
			discardIds.push_back(i);
		}
	}

	if (discardIds.empty())
	{
		return;
	}

	// Sort by sortOrder to maintain the order cards were discarded
	std::sort(discardIds.begin(), discardIds.end(), [&](int a, int b) {
		return state.cards[a].sortOrder < state.cards[b].sortOrder;
	});

	for (int i { 0 }; i < discardIds.size(); i++)
	{
		int id { discardIds[i] };
		float offset { i * m_stackOffset };

		state.cards[id].targetPosition = {
			m_bounds.x + offset,
			m_bounds.y - offset,
			0
		};

		if (state.drag.isActive && state.drag.cardId == id)
		{
			// dragged card on top
			state.cards[id].sortOrder = 999;
		}
		else
		{
			state.cards[id].sortOrder = i;
		}

		state.cards[id].isFaceUp = true;
	}
}

void DiscardZone::Draw()
{
	DrawRectangleRounded(m_bounds, 0.1f, 10, Fade(BLACK, 0.35f));
}
