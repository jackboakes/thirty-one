#include "hand.h"

#include <algorithm>

Hand::Hand()
{
	size = { 456.0f, 232.0f };
}

void Hand::OnUpdate(float deltaTime)
{
	// bubble sort based on x positions
	// TODO:: place this in a function
	Card* draggedCard { nullptr };

	for (auto& child : m_Children)
	{
		Card* card { static_cast<Card*>(child.get()) };
		if (card->isDragging)
		{
			draggedCard = card;
			break;
		}
	}

	if (draggedCard)
	{
		// Sort by X position
		std::sort(m_Children.begin(), m_Children.end(),
			[](const std::unique_ptr<Element>& a, const std::unique_ptr<Element>& b) {
				return a->worldPosition.x < b->worldPosition.x;
			});

		// Sync localPosition to match the mouse-controlled worldPosition
		draggedCard->localPosition.x = draggedCard->worldPosition.x - worldPosition.x;
		draggedCard->localPosition.y = draggedCard->worldPosition.y - worldPosition.y;

		// Apply new order
		Layout();
	}
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
	Rectangle bgRect { worldPosition.x, worldPosition.y, size.x, size.y };

	DrawRectangleRounded(bgRect, 0.1f, 10, Fade(BLACK, 0.35f));
}

Card* Hand::GetCardAt(Vector2 worldPosition)
{
	for (auto it { m_Children.rbegin() }; it != m_Children.rend(); ++it)
	{
		Element* element { it->get() };
		Card* card { static_cast<Card*>(element) };

		Rectangle rec { card->worldPosition.x, card->worldPosition.y, card->size.x, card->size.y };

		if (CheckCollisionPointRec(worldPosition, rec))
		{
			return card;
		}
	}
	return nullptr;
}