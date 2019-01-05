#include <entity.h>
#include <model.h>

namespace GW {
	Entity::Entity(std::string name)
	{
		m_name = name;
		m_world = nullptr;
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