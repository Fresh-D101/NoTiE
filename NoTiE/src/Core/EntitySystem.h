#pragma once

#include <array>
#include "EntityId.h"
#include "Component.h"
#include "ComponentPool.h"
#include "System.h"

namespace sf
{
	class Event;
}

class EntitySystem
{
public:

	~EntitySystem()
	{
		for (int i = 0; i < pools.size(); i++)
		{
			delete pools[i];
		}
	}

	static EntitySystem& GetInstance();
	
	EntityId AddEntity();
	Entity* GetEntityByID(EntityId _id);
	bool DestroyEntity(EntityId _id);

	void AddSystem(System& system);
	void Update();
	void HandleEvent(sf::Event& event);

	template <typename ComponentType>
	ComponentType* AddComponent(EntityId entityId)
	{
		Entity* entity = GetEntityByID(entityId);
		if (!entity)
		{
			return nullptr;
		}

		int componentId = Component<ComponentType>::GetId();

		if (componentId >= pools.size())
		{
			pools.resize(componentId + 1, nullptr);
			pools[componentId] = new ComponentPool<ComponentType>();
		}
		else if(pools[componentId] == nullptr)
		{
			pools[componentId] = new ComponentPool<ComponentType>();
		}

		if (entity->HasComponent(componentId))
		{
			return nullptr;
		}

		pools[componentId]->AddEntity(entityId);
		entity->AddComponent(componentId);

		for (int i = 0; i < systems.size(); i++)
		{
			std::bitset<32>& components = entity->Components;
			const std::bitset<32>& requiredComponents = systems[i]->GetRequiredComponents();

			bool qualified = (components & requiredComponents) == requiredComponents;

			if (qualified && !systems[i]->HasEntity(entityId))
			{
				systems[i]->AddEntity(entityId);
			}
		}

		return GetComponent<ComponentType>(entityId);
	}

	template <typename ComponentType>
	bool RemoveComponent(EntityId entityId)
	{
		Entity* entity = GetEntityByID(entityId);
		if (!entity)
		{
			return false;
		}

		int componentId = Component<ComponentType>::GetId();

		if (componentId >= pools.size())
		{
			return false;
		}

		if (!entity->HasComponent(componentId))
		{
			return false;
		}

		entity->RemoveComponent(componentId);

		for (int i = 0; i < systems.size(); i++)
		{
			if (!systems[i]->IsEntityQualified(entity->Components) && systems[i]->HasEntity(entityId))
			{
				systems[i]->RemoveEntity(entityId);
			}
		}

		return true;
	}

	template <typename ComponentType>
	bool HasComponent(EntityId entityId)
	{
		return GetComponent<ComponentType>(entityId) != nullptr;
	}

	template <typename ComponentType>
	ComponentType* GetComponent(EntityId entityId)
	{
		Entity* entity = GetEntityByID(entityId);
		if (!entity)
		{
			return nullptr;
		}

		int componentId = Component<ComponentType>::GetId();

		if (componentId >= pools.size())
		{
			return nullptr;
		}

		if (!entity->HasComponent(componentId))
		{
			return nullptr;
		}

		void* aliasedProperty = pools[componentId]->Get(entityId);
		ComponentType* property = static_cast<ComponentType*>(aliasedProperty);

		return property;
	}

	
	
private:

	static EntitySystem Instance;

	IndexType nextInsertionIndex = 1;
	std::array<Entity, (std::numeric_limits<IndexType>::max)()> entities;

	std::vector<System*> systems;
	std::vector<ComponentPoolBase*> pools;
};
