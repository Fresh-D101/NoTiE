#pragma once
#include "../Core/System.h"
#include "../Core/Window.h"

class RenderSystem : public System
{
public:
	RenderSystem();

	void Begin();
	void Draw();
	void End();

	Window& GetWindow();

	static Window* GetActiveWindow() { return ActiveWindow; }

private:

	Window window;

	static Window* ActiveWindow;
};