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
		screenPosition.x = m_Parent->screenPosition.x + localPosition.x;
		screenPosition.y = m_Parent->screenPosition.y + localPosition.y;
	}
	else
	{
		screenPosition = localPosition;
	}

	OnLayout();

	for (auto& child : m_Children)
	{
		child->Layout();
	}
}