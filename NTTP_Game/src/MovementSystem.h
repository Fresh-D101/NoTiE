#pragma once
#include "Core/System.h"

struct MovementComponent
{
	float Speed;
};

class MovementSystem : public System
{
public:
	MovementSystem();
	~MovementSystem() = default;
	
	void Update() override;
};
