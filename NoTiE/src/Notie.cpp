#include "Notie.h"
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "ImGui/imgui-NoTiE.h"
#include "Resources/TextureManager.h"

Notie::Notie(int windowWidth, int windowHeight, std::string text, bool fullscreen) 
	: entitySystem(EntitySystem::GetInstance())
{
	m_IsRunning = true;

	//Set Up RenderSystem
	renderSystem = new RenderSystem();
	entitySystem.AddSystem(*renderSystem);

    TextureManager::instance().StartUp("/graphics");

    isoEngine = new IsoEngine();

	//Initialize ImGui
    ImGui::notie::Init(renderSystem->GetWindow());
}

Notie::~Notie()
{
    TextureManager::instance().ShutDown();

    ImGui::notie::Shutdown();
	
	delete renderSystem;
	m_IsRunning = false;
}

void Notie::Update()
{
	if (!renderSystem->GetWindow().Update())
	{
        m_IsRunning = false;
        return;
	}
}

void Notie::RunApplication(Application& app)
{
    //Call the initial Setup for Application
    app.Setup(*this);

    while (renderSystem->GetWindow().Update())
    {
        //Update ImGui
        ImGui::notie::Update(renderSystem->GetWindow(), Time::DeltaTime());
    	
    	//Update all Iso related systems
        isoEngine->Update();
    	
    	//Update all Systems
        entitySystem.Update();
    	
    	//Start Drawing
        renderSystem->Begin();
        //render the Map and everything about the Isometric Engine
        isoEngine->Draw(*renderSystem);
    	//Render all entities
		renderSystem->Draw();
    	//render Gui
        ImGui::notie::Render(renderSystem->GetWindow().GetRenderWindow());
        //Display
        renderSystem->End();

    	//TODO: improve Delta Time approach
        Time::ResetTimer();
    }
	
    m_IsRunning = false;
}



void Notie::ShowTestWindow()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}