#pragma once
#include "EntityId.h"
#include <vector>

class ComponentPoolBase
{
public:

	virtual void AddEntity(EntityId entityId) = 0;
	virtual void* Get(EntityId entityId) = 0;

	virtual ~ComponentPoolBase() {}
};

template <typename ComponentType>
class ComponentPool : public ComponentPoolBase
{
public:

	ComponentPool()
	{
		components.reserve(100);
	}

	void AddEntity(EntityId entityId) override
	{
		if (entityId.index >= components.size())
		{
			components.resize(entityId.index + 1);
			return;
		}

		components[entityId.index] = ComponentType();
	}
	
	virtual void* Get(EntityId entityId) override
	{
		if (entityId.index >= components.size())
		{
			return nullptr;
		}

		return &components[entityId.index];
	}
	
private:
	std::vector<ComponentType> components;
};