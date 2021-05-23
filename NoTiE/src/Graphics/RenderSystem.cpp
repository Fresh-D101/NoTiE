#include "RenderSystem.h"
#include <SFML/Graphics.hpp>

Window* RenderSystem::ActiveWindow = nullptr;

RenderSystem::RenderSystem() : 
	window(1920, 1080, "NoTitleEngine", false)
{
	AddRequiredComponent<Sprite>();
	ActiveWindow = &window;
}

void RenderSystem::Begin()
{
	window.BeginDraw();
}

void RenderSystem::Draw()
{
	EntitySystem& entitySystem = EntitySystem::GetInstance();

	for (int i = 0; i < registeredEntities.size(); i++)
	{
		Sprite* spriteComponent = entitySystem.GetComponent<Sprite>(registeredEntities[i]);
		window.DrawSprite(*spriteComponent);
	}
}

void RenderSystem::End()
{
	window.Display();
}

Window& RenderSystem::GetWindow()
{
	return window;
}