#include "element.h"
#include "card.h"

class Hand : public Element {
public:
	Hand();
	void OnLayout() override;
	void OnRender() override;
	void AddCard(std::unique_ptr<Card> card);
	void AddChild(std::unique_ptr<Element> child) = delete;
	Card* GetCardAt(Vector2 screenPosition);

	void UpdateSort(Element* draggedCard);

private:
	float m_CardWidth { 0.0f };
	float m_OverlapRatio { 0.25f };
};