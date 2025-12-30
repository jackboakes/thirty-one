#include "hand.h"

#include <algorithm>

Hand::Hand()
{
	size = { 456.0f, 232.0f };
}

void Hand::AddCard(std::unique_ptr<Card> card)
{
	if (m_Children.empty())
	{
		m_CardWidth = card->size.x;
	}

	Card* cardPtr { card.get() };
	Element::AddChild(std::move(card));

	Layout();

	if (cardPtr)
	{
		cardPtr->SnapToPosition();
	}
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

		// if card is dragging skip layout update
		const Card* cardPtr { dynamic_cast<Card*>(child.get()) };
		if (cardPtr && cardPtr->isDragging)
		{
			continue;
		}

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

Card* Hand::GetCardAt(Vector2 screenPosition)
{
	for (auto it { m_Children.rbegin() }; it != m_Children.rend(); ++it)
	{
		Element* element { it->get() };
		Card* card { static_cast<Card*>(element) };

		Rectangle rec { card->screenPosition.x, card->screenPosition.y, card->size.x, card->size.y };

		if (CheckCollisionPointRec(screenPosition, rec))
		{
			return card;
		}
	}
	return nullptr;
}

void Hand::UpdateSort(Element* draggedCard)
{
	std::sort(m_Children.begin(), m_Children.end(),
		[](const std::unique_ptr<Element>& a, const std::unique_ptr<Element>& b) {
			return a->screenPosition.x < b->screenPosition.x;
		});

	if (draggedCard)
	{
		draggedCard->localPosition.x = draggedCard->screenPosition.x - screenPosition.x;
		draggedCard->localPosition.y = draggedCard->screenPosition.y - screenPosition.y;
	}

	Layout();

}