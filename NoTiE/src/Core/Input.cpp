#include "Input.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include "Window.h"

bool Input::IsMousePressed(MouseButton button)
{
	return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button));
}

Vector2Int Input::GetMousePositionInWindow(const Window& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window.GetRenderWindow());
	return Vector2Int(mousePos.x, mousePos.y);
}

void Input::SetMousePositionInWindow(const Vector2Int& position, const Window& window)
{
	sf::Mouse::setPosition(sf::Vector2i(position.x, position.y), window.GetRenderWindow());
}

void Input::SetMousePosition(const Vector2Int& position)
{
	sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
}

bool Input::IsKeyPressed(Key key)
{
	return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}
