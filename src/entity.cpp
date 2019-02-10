#include <entity.h>
#include <model.h>

namespace GW {
	Entity::Entity()
	{
		m_world = nullptr;
		rootComponent = nullptr;
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