#pragma once
#include "raylib.h"
#include <vector>
#include <memory>

class Element {
public:

	void Update(float deltaTime);
	void Render();

	void AddChild(std::unique_ptr<Element> child);
	std::unique_ptr<Element> RemoveChild(Element* childToRemove);
	const std::vector<std::unique_ptr<Element>>& GetChildren() const { return m_Children; }
	void Layout();

	// Relative to parent
	Vector2 localPosition { 0.0f, 0.0f };
	// Width/Height
	Vector2 size { 0.0f, 0.0f };
	// Calculated absolute world position
	Vector2 screenPosition { 0.0f, 0.0f };

	bool isVisible { true };

protected:
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnLayout() {}


	std::vector<std::unique_ptr<Element>> m_Children;
	Element* m_Parent { nullptr };
};