#pragma once
#include "Core/Application.h"
#include "Core/Window.h"


class TestApp : public Application
{
public:
	void Setup(Notie& engine) override;
	void Update() override {}
	void Draw(Window& window) override {}
};
