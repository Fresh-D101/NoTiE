#pragma once

class ComponentBase
{
protected:

	static int Counter;
};

template <typename ComponentType>
class Component : public ComponentBase
{
public:

	static int GetId()
	{
		static int Id = Counter++;
		return Id;
	}
};