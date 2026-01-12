#include "handzone.h"
#include "card.h"
#include "globals.h"
#include <algorithm>


HandZone::HandZone()
{
	const Vector2 cardSize { CardRenderer::GetSize() };
	const float cardSpacing { cardSize.x * m_cardSpacing };

	float totalCardWidth { ((m_capacity - 1) * cardSpacing) + cardSize.x };

	float containerWidth { totalCardWidth + (m_padding * 2) };
	float containerHeight { cardSize.y };

	m_bounds = {
		(GameResolution::f_Width - containerWidth) * 0.5f,
		GameResolution::f_Height - containerHeight - 50.0f,
		containerWidth,
		containerHeight
	};
}

void HandZone::UpdateLayout(GameState& state)
{
	std::vector<int> handIds;
	for (int i { 0 }; i < state.cards.size(); i++)
	{
		if (state.cards[i].zone == CardZone::Hand) 
		{
			handIds.push_back(i);
		}
	}

	if (handIds.empty())
	{
		return;
	}

	// This gives a bubble sort effect to the hand when dragging a card around
	std::sort(handIds.begin(), handIds.end(), [&](int a, int b) {
		return state.cards[a].position.x < state.cards[b].position.x;
		});

	const Vector2 cardSize { CardRenderer::GetSize() };
	const float cardSpacing { cardSize.x * m_cardSpacing };
	const float currentHandWidth { ((static_cast<float>(handIds.size()) - 1.0f) * cardSpacing) + cardSize.x };

	// Centre the cards within fixed bounds
	const float startX { m_bounds.x + (m_bounds.width * 0.5f) - (currentHandWidth * 0.5f) };
	const float centreY { m_bounds.y };

	for (int i { 0 }; i < handIds.size(); i++)
	{
		int id { handIds[i] };
		state.cards[id].targetPosition = { startX + (i * cardSpacing), centreY, 0 };

		// Z ordering
		if (state.drag.isActive && state.drag.cardId == id)
		{
			// dragged card on top
			state.cards[id].sortOrder = 999;
		}
		else
		{
			state.cards[id].sortOrder = i;
		}
	}
}

void HandZone::Draw()
{
	DrawRectangleRounded(m_bounds, 0.1f, 10, Fade(BLACK, 0.35f));
	// Show exact bounds
	//DrawRectangleLines(m_bounds.x, m_bounds.y, m_bounds.width, m_bounds.height, RED);
}

bool HandZone::IsPointInside(Vector2 worldPosition) const
{
	return CheckCollisionPointRec(worldPosition, m_bounds);
}