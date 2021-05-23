#include "Camera.h"

#include <SFML/Graphics/View.hpp>

#include "RenderSystem.h"

Camera::Camera()
{
	NativeView = std::make_unique<sf::View>();
}

Camera::Camera(Vector2Float origin, Vector2Float size)
{
	NativeView = std::make_unique<sf::View>(sf::Vector2f(origin.x, origin.y), sf::Vector2f(size.x, size.y));
}

Camera::Camera(const Camera& other)
{
	NativeView = std::make_unique<sf::View >(*other.NativeView);
}

Camera& Camera::operator=(const Camera& other)
{
	NativeView = std::make_unique<sf::View>(*other.NativeView);
	return *this;
}

Camera::~Camera() = default;

void Camera::Resize(Vector2Float origin, Vector2Float size)
{
	NativeView->reset(sf::FloatRect(origin.x, origin.y, size.x, size.y));
}

void Camera::SetCenter(Vector2Float center)
{
	NativeView->setCenter(center.x, center.y);
}

void Camera::SetSize(Vector2Float size)
{
	NativeView->setSize(size.x, size.y);
}

void Camera::Zoom(float factor)
{
	NativeView->zoom(factor);
}

void Camera::ZoomAt(Vector2Int pixel, Window& window, float factor)
{
	const Vector2Float beforeCoord{ window.ScreenToWorld(pixel) };
	Zoom(factor);
	window.SetCamera(*this);
	const Vector2Float afterCoord{ window.ScreenToWorld(pixel) };
	const Vector2Float offsetCoords{ beforeCoord - afterCoord };
	Move(offsetCoords);
	window.SetCamera(*this);
}


void Camera::Move(Vector2Float position)
{
	NativeView->move(position.x, position.y);
}


sf::View& Camera::GetNativeView() const
{
	return *NativeView.get();
}



