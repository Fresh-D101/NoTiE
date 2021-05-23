#include "CameraSystem.h"

#include "Camera.h"
#include "RenderSystem.h"
#include "../Core/EntitySystem.h"
#include "../Core/Input.h"

CameraSystem::CameraSystem() : EntitySystem(EntitySystem::GetInstance()), ActiveWindow(*RenderSystem::GetActiveWindow())
{
	AddRequiredComponent<Camera>();
	ButtonHeld = false;
}

void CameraSystem::Update()
{
	if (ButtonHeld)
	{
		const Vector2Float mousePos = Input::GetMousePositionInWindow(ActiveWindow);
		for (int i = 0; i < registeredEntities.size(); i++)
		{
			Camera* camComponent = EntitySystem.GetComponent<Camera>(registeredEntities[i]);
			camComponent->Move((mousePos - CachedMousePos) * PanSpeed);
			ActiveWindow.SetCamera(*camComponent);
		}
		Input::SetMousePositionInWindow(CachedMousePos, ActiveWindow);
	}
}

void CameraSystem::OnMousePressed(MouseButton button)
{
	if(button == MouseButton::Right)
	{
		Vector2Float mousePos = Input::GetMousePositionInWindow(ActiveWindow);
		CachedMousePos = mousePos;
		ActiveWindow.ShowCursor(false);
		ButtonHeld = true;
	}
}

void CameraSystem::OnMouseReleased(MouseButton button)
{
	if(button == MouseButton::Right)
	{
		ButtonHeld = false;
		Input::SetMousePositionInWindow(CachedMousePos, ActiveWindow);
		ActiveWindow.ShowCursor(true);
	}
}

void CameraSystem::OnMouseWheelScrolled(ScrollEventData& eventData)
{
	if (eventData.delta > 0)
	{
		for (int i = 0; i < registeredEntities.size(); i++)
		{
			Camera* camComponent = EntitySystem.GetComponent<Camera>(registeredEntities[i]);
			camComponent->ZoomAt({ eventData.x, eventData.y }, ActiveWindow, (1.f / ZoomFactor));
		}
	}
	else if (eventData.delta < 0)
	{
		for (int i = 0; i < registeredEntities.size(); i++)
		{
			Camera* camComponent = EntitySystem.GetComponent<Camera>(registeredEntities[i]);
			camComponent->ZoomAt({ eventData.x, eventData.y }, ActiveWindow,  ZoomFactor);
		}
	}
}