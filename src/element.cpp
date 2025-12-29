#include "element.h"
#include <algorithm>

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

std::unique_ptr<Element> Element::RemoveChild(Element* childToRemove)
{
	auto it { std::find_if(m_Children.begin(), m_Children.end(),
		[childToRemove](const std::unique_ptr<Element>& ptr) {
			return ptr.get() == childToRemove;
	}) };

	if (it == m_Children.end())
	{
		return nullptr;
	}
	std::unique_ptr<Element> removedChild = std::move(*it);
	m_Children.erase(it);
	return removedChild;
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