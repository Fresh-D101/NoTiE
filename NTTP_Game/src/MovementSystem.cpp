#include "MovementSystem.h"


#include "Core/EntitySystem.h"
#include "Core/Time.h"
#include "Graphics/Sprite.h"

MovementSystem::MovementSystem()
{
	AddRequiredComponent<Sprite>();
	AddRequiredComponent<MovementComponent>();
}

void MovementSystem::Update()
{
	EntitySystem& entitySystem = EntitySystem::GetInstance();
	auto dt = Time::DeltaTime();
	
	for (int i = 0; i < registeredEntities.size(); i++)
	{
		Sprite* sprite = entitySystem.GetComponent<Sprite>(registeredEntities[i]);
		MovementComponent* movement = entitySystem.GetComponent<MovementComponent>(registeredEntities[i]);
		sprite->Move(movement->Speed * dt, 0);

		auto pos = sprite->GetPosition();
		if (pos.x > 1920)
		{
			sprite->SetPosition(-20, pos.y);
		}
	}
}

