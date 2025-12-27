#include "hand.h"

Hand::Hand()
{
	size = { 300.0f, 160.0f };
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
	float xStride = m_CardWidth * (1.0f - m_OverlapRatio);
	float totalGroupWidth = (xStride * (m_Children.size() - 1)) + m_CardWidth;
	float centerOfHand = size.x / 2.0f;
	float startX = centerOfHand - (totalGroupWidth / 2.0f);

	for (int i { 0 }; i < m_Children.size(); i++)
	{
		auto& child = m_Children[i];

		float newX = startX + (i * xStride);
		float newY = (size.y - child->size.y) / 2.0f; 
		child->localPosition = { newX, newY };
	}
}

void Hand::OnRender()
{
	Rectangle bgRect { screenPosition.x, screenPosition.y, size.x, size.y };

	DrawRectangleRounded(bgRect, 0.1f, 10, Fade(BLACK, 0.35f));
}