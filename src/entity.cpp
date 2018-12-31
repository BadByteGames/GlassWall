#include <entity.h>

namespace GW {
	Entity::Entity(std::string name)
	{
		m_name = name;
	}

	Entity::~Entity()
	{
	}

	void Entity::entityStart()
	{
	}

	void Entity::onWorldStart()
	{
	}

	void Entity::update()
	{
	}

	void Entity::cleanUp()
	{
	}

}