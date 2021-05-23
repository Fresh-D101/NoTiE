#include "System.h"

void System::AddEntity(EntityId entityId)
{
	registeredEntities.push_back(entityId);
}

void System::RemoveEntity(EntityId entityId)
{
	for (int i = 0; i < registeredEntities.size(); i++)
	{
		if (registeredEntities[i] == entityId)
		{
			registeredEntities.erase(registeredEntities.begin() + i);
			return;
		}
	}
}

bool System::HasEntity(EntityId entityId)
{
	for (int i = 0; i < registeredEntities.size(); i++)
	{
		if (registeredEntities[i] == entityId)
		{
			return true;
		}
	}

	return false;
}


