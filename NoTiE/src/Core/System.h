#pragma once


#include <bitset>
#include <vector>

#include "EntityId.h"
#include "Component.h"
#include "Input.h"

enum class Key;

struct NoEntitiesAllowedTag {};

class System
{
public:
	virtual ~System() = default;

	virtual void AddEntity(EntityId entityId);
	virtual void RemoveEntity(EntityId entityId);
	virtual bool HasEntity(EntityId entityId);

	virtual void Init() { };
	virtual void Update() { };

	const std::bitset<32>& GetRequiredComponents() const
	{
		return requiredComponents;
	}

	bool IsEntityQualified(std::bitset<32>& components) const
	{
		return (components & requiredComponents) == requiredComponents;
	}

	//Input Event Functions
	virtual void OnKeyPressed(Key key) {};
	virtual void OnKeyReleased(Key key) {};
	virtual void OnMousePressed(MouseButton button) {};
	virtual void OnMouseReleased(MouseButton button) {};
	virtual void OnMouseWheelScrolled(ScrollEventData& eventData) {};
protected:

	template <typename ComponentType>
	void AddRequiredComponent()
	{
		int componentId = Component<ComponentType>::GetId();
		requiredComponents.set(componentId, true);
	}

	std::bitset<32> requiredComponents;
	std::vector<EntityId> registeredEntities;
};
