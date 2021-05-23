#pragma once
#include <memory>
#include <string>



#include "EntitySystem.h"
#include "Vector2.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Sprite.h"


namespace sf
{
	class RenderWindow;
}

class Window
{
public:
	Window(const int width, const int height, const std::string title, const bool fullscreen);
	~Window() = default;

public:
	bool Update() const;

	//Rendering:
	void BeginDraw() const;
	void DrawSprite(Sprite& sprite) const;
	void Display() const;
	
	void SetCamera(Camera& camera);
	void ShowCursor(bool show);
	bool HasFocus();

	Vector2Float ScreenToWorld(Vector2Int screenPos) const;
	Vector2Int WorldToScreen(Vector2Float worldPos) const;

	Vector2Int GetSize() const;
	//Get the underlying SFML RenderWindow
	sf::RenderWindow& GetRenderWindow() const;

private:
	std::unique_ptr<sf::RenderWindow> NativeWindow;
	EntitySystem& EntitySystem;
};
