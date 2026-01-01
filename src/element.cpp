#include "element.h"
#include <algorithm>

void Element::Update(float deltaTime)
{
	OnUpdate(deltaTime);

	for (auto& child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void Element::Render()
{
	OnRender();
	for (auto& child : m_Children)
	{
		if (child->isVisible)
		{
			child->Render();
		}
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
		worldPosition.x = m_Parent->worldPosition.x + localPosition.x;
		worldPosition.y = m_Parent->worldPosition.y + localPosition.y;
	}
	else
	{
		worldPosition = localPosition;
	}

	OnLayout();

	for (auto& child : m_Children)
	{
		child->Layout();
	}
}