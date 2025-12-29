#include "hand.h"

Hand::Hand()
{
	size = { 456.0f, 232.0f };
}

void Hand::AddCard(std::unique_ptr<Element> card)
{
	if (m_Children.empty())
	{
		m_CardWidth = card->size.x;
	}

	AddChild(std::move(card));

	Layout();
}

void Hand::OnLayout()
{
	if (m_Children.empty())
	{
		return;
	}
	// calculate overlap
	const float cardSpacingX { m_CardWidth * (1.0f - m_OverlapRatio) };
	const float totalCardsWidth { (cardSpacingX * (m_Children.size() - 1)) + m_CardWidth };
	const float handCentreX { size.x / 2.0f };
	const float firstCardX { handCentreX - (totalCardsWidth / 2.0f) };

	for (int i { 0 }; i < m_Children.size(); i++)
	{
		auto& child { m_Children[i] };

		const float cardPositionX { firstCardX + (i * cardSpacingX) };
		const float cardPositionY { (size.y - child->size.y) / 2.0f };
		child->localPosition = { cardPositionX, cardPositionY };
	}
}

void Hand::OnRender()
{
	Rectangle bgRect { screenPosition.x, screenPosition.y, size.x, size.y };

	DrawRectangleRounded(bgRect, 0.1f, 10, Fade(BLACK, 0.35f));
}

Element* Hand::GetCardAt(Vector2 screenPosition)
{
	for (auto it { m_Children.rbegin() }; it != m_Children.rend(); ++it)
	{
		Element* card { it->get() };
		Rectangle rec { card->screenPosition.x, card->screenPosition.y, card->size.x, card->size.y };

		if (CheckCollisionPointRec(screenPosition, rec))
		{
			return card;
		}
	}
	return nullptr;
}