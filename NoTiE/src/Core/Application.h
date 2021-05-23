#pragma once

class Window;
class Notie;

class Application
{
public:
	virtual ~Application() = default;
	virtual void Setup(Notie& engine) = 0;
	virtual void Update() = 0;
	virtual void Draw(Window& window) = 0;
};
