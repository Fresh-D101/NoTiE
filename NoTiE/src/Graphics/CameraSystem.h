#pragma once
#include "../Core/EntitySystem.h"
#include "../Core/System.h"
#include "../Core/Vector2.h"

class CameraSystem : public System
{
public:
	CameraSystem();
	
	void Update() override;
	void OnMousePressed(MouseButton button) override;
	void OnMouseReleased(MouseButton button) override;
	void OnMouseWheelScrolled(ScrollEventData& eventData) override;

private:
	EntitySystem& EntitySystem;
	Window& ActiveWindow;
	Vector2Int CachedMousePos;
	bool ButtonHeld;
	float PanSpeed = 0.6f;
	float ZoomFactor = 1.1f;
};
