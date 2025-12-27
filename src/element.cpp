#include "element.h"

void Element::Update(float deltaTime)
{

}

void Element::Render()
{
	OnRender();
	for (auto& child : m_Children)
	{
		child->Render();
	}
}

void Element::AddChild(std::unique_ptr<Element> child)
{
	child->m_Parent = this;
	m_Children.push_back(std::move(child));
}

void Element::Layout()
{
	if (m_Parent)
	{
		screenPos.x = m_Parent->screenPos.x + position.x;
		screenPos.y = m_Parent->screenPos.y + position.y;
	}
	else
	{
		screenPos = position;
	}

	OnLayout();

	for (auto& child : m_Children)
	{
		child->Layout();
	}
}