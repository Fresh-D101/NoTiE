#pragma once

#include "Core/Application.h"
#include "Graphics/RenderSystem.h"
#include "Iso/IsoEngine.h"

class Notie
{
public:
	Notie(int windowWidth, int windowHeight, std::string text, bool fullscreen);
	~Notie();

public:
	const bool IsRunning() const { return m_IsRunning; }
	void Update();
	void RunApplication(Application& app);

	IsoEngine* GetIsoEngine() const { return isoEngine; }
	
	void ShowTestWindow();

private:
	bool m_IsRunning;

	EntitySystem& entitySystem;
	RenderSystem* renderSystem;
	IsoEngine* isoEngine;

	//LayerStack layerStack;
};
