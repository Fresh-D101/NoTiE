#pragma once
#include <memory>
#include "../Core/Vector2.h"

namespace sf
{
	class View;
}

class Window;

class Camera
{
public:
	Camera();
	Camera(Vector2Float origin, Vector2Float size);

	Camera(const Camera& other);
	Camera& operator=(const Camera& other);
	
	~Camera();

	void Resize(Vector2Float origin, Vector2Float size);
	void SetCenter(Vector2Float center);
	void SetSize(Vector2Float size);
	void Zoom(float factor);
	void ZoomAt(Vector2Int pixel, Window& window, float factor);
	void Move(Vector2Float position);

	sf::View& GetNativeView() const;
	
private:
	std::unique_ptr<sf::View> NativeView;
};