#include "EntityId.h"

bool Entity::IsAlive() const
{
	return Id.index != EntityId::InvalidEntityId.index;
}

bool Entity::HasComponent(int componentId) const
{
	return Components.test(componentId);
}

void Entity::AddComponent(int componentId)
{
	Components.set(componentId, true);
}

void Entity::RemoveComponent(int componentId)
{
	Components.set(componentId, false);
}

