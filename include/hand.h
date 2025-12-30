#include "element.h"
#include "card.h"

class Hand : public Element {
public:
	Hand();
	void AddChild(std::unique_ptr<Element> child) = delete;
	void AddCard(std::unique_ptr<Card> card);
	Card* GetCardAt(Vector2 screenPosition);

protected:
	void OnUpdate(float deltaTime) override;
	void OnLayout() override;
	void OnRender() override;

private:
	float m_CardWidth { 0.0f };
	float m_OverlapRatio { 0.25f };
};