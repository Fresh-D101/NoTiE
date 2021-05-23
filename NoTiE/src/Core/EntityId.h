#pragma once
#include <bitset>

using IndexType = unsigned short;
using SaltType = unsigned short;

struct EntityId
{
	EntityId()
		: index(0)
		, salt(0)
	{
	}

	EntityId(IndexType _index, SaltType _salt)
		: index(_index)
		, salt(_salt)
	{
	}

	IndexType index;
	SaltType salt;

	bool operator==(EntityId& rhs) const
	{
		return index == rhs.index && salt == rhs.salt;
	}

	bool operator !=(EntityId& rhs) const
	{
		return !(*this == rhs);
	}
	
	static EntityId InvalidEntityId;
};

class Entity
{
public:
	std::string Name;
	std::bitset<32> Components;
	EntityId Id;

	bool IsAlive() const;
	bool HasComponent(int componentId) const;
	void AddComponent(int componentId);
	void RemoveComponent(int componentId);
};