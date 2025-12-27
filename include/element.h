#pragma once
#include "raylib.h"
#include <vector>
#include <memory>

class Element {
public:

	void Update(float deltaTime);
	void Render();

	void AddChild(std::unique_ptr<Element> child);
	void Layout();

	// Relative to parent
	Vector2 localPosition { 0.0f, 0.0f };
	// Width/Height
	Vector2 size { 0.0f, 0.0f };
	// Calculated absolute world position
	Vector2 screenPosition { 0.0f, 0.0f };

protected:
	virtual void OnRender() = 0;
	virtual void OnLayout() = 0;

	std::vector<std::unique_ptr<Element>> m_Children;
	Element* m_Parent { nullptr };
};