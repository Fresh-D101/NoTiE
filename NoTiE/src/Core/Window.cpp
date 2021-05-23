#include "Window.h"
#include <SFML/Graphics.hpp>

#include "../ImGui/imgui-NoTiE.h"

Window::Window(const int width, const int height, const std::string title, const bool fullscreen)
	: EntitySystem(EntitySystem::GetInstance())
{
	sf::VideoMode mode;
	mode.height = height;
	mode.width = width;
	int style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

	NativeWindow = std::make_unique<sf::RenderWindow>(mode, title, style);
}

bool Window::Update() const
{
	sf::Event event;
	while (NativeWindow->pollEvent(event))
	{
		if(!ImGui::notie::ProcessEvent(event))
		{
			EntitySystem.HandleEvent(event);
		};
		
		if(event.type == sf::Event::Closed)
		{
			return false;
		}
	}
	return true;
}

void Window::BeginDraw() const
{
	NativeWindow->clear();
}


void Window::DrawSprite(Sprite& sprite) const
{
	NativeWindow->draw(sprite.GetSprite());
}

void Window::Display() const
{
	NativeWindow->display();
}

Vector2Int Window::GetSize() const
{
	sf::Vector2u size = NativeWindow->getSize();
	return Vector2Int(size.x, size.y);
}

sf::RenderWindow& Window::GetRenderWindow() const
{
	return *NativeWindow;
}

void Window::SetCamera(Camera& camera)
{
	NativeWindow->setView(camera.GetNativeView());
}

void Window::ShowCursor(bool show)
{
	NativeWindow->setMouseCursorVisible(show);
}

bool Window::HasFocus()
{
	return NativeWindow->hasFocus();
}


Vector2Float Window::ScreenToWorld(Vector2Int screenPos) const
{
	const sf::Vector2f nativeCoords = NativeWindow->mapPixelToCoords(sf::Vector2i(screenPos.x, screenPos.y));
	return Vector2Float(nativeCoords.x, nativeCoords.y);
}

Vector2Int Window::WorldToScreen(Vector2Float worldPos) const
{
	const sf::Vector2i nativePixel = NativeWindow->mapCoordsToPixel(sf::Vector2f(worldPos.x, worldPos.y));
	return Vector2Int(nativePixel.x, nativePixel.y);
}
