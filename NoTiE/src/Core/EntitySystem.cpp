#include "EntitySystem.h"
#include <SFML/Graphics.hpp>

EntityId EntityId::InvalidEntityId = EntityId();
EntitySystem EntitySystem::Instance = EntitySystem();

///////////////////////////////////////////////////////////////
EntitySystem& EntitySystem::GetInstance()
{
	return Instance;
}

EntityId EntitySystem::AddEntity()
{
	for (int i = nextInsertionIndex; i < entities.size(); i++)
	{
		const bool isAlive = entities[i].IsAlive();
		if (!isAlive)
		{
			EntityId newId(i, entities[i].Id.salt + 1);
			entities[i].Id = newId;

			nextInsertionIndex = i + 1;
			if (nextInsertionIndex >= std::numeric_limits<IndexType>::max())
			{
				nextInsertionIndex = 1;
			}

			return newId;
		}
	}

	return EntityId();
}

Entity* EntitySystem::GetEntityByID(EntityId _id)
{
	if (_id.index == 0)
	{
		return nullptr;
	}
	
	if (entities[_id.index].IsAlive() && entities[_id.index].Id.salt == _id.salt)
	{
		return &entities[_id.index];
	}

	return nullptr;
}

bool EntitySystem::DestroyEntity(EntityId _id)
{
	Entity* entity = GetEntityByID(_id);
	if (entity != nullptr)
	{
		entity->Id.index = 0;

		return true;
	}

	return false;
}

void EntitySystem::AddSystem(System& system)
{
	systems.push_back(&system);	
	system.Init();
}

void EntitySystem::Update()
{
	for (int i = 0; i < systems.size(); i++)
	{
		systems[i]->Update();
	}
}

void EntitySystem::HandleEvent(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			for (auto& system : systems)
			{
				system->OnKeyPressed(static_cast<Key>(event.key.code));
			}
			break;
		
		case sf::Event::KeyReleased:
			for (auto& system : systems)
			{
				system->OnKeyReleased(static_cast<Key>(event.key.code));
			}
			break;

		case sf::Event::MouseButtonPressed:
			for (auto& system : systems)
			{
				system->OnMousePressed(static_cast<MouseButton>(event.mouseButton.button));
			}
			break;

		case sf::Event::MouseButtonReleased:
			for (auto& system : systems)
			{
				system->OnMouseReleased(static_cast<MouseButton>(event.mouseButton.button));
			}
			break;

		case sf::Event::MouseWheelScrolled:
			auto& eventData = event.mouseWheelScroll;
			ScrollEventData data = {
				static_cast<Wheel>(eventData.wheel),
				eventData.delta,
				eventData.x,
				eventData.y
			};
		
			for (auto& system : systems)
			{
				system->OnMouseWheelScrolled(data);
			}
			break;
	}
}


