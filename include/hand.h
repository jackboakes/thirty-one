#include "element.h"

class Hand : public Element {
public:
	Hand();
	void OnLayout() override;
	void OnRender() override;
	void AddCard(std::unique_ptr<Element> card);
	Element* GetCardAt(Vector2 screenPosition);

private:
	float m_CardWidth { 0.0f };
	float m_OverlapRatio { 0.25f };
};