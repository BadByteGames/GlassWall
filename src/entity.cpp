#include <entity.h>
#include <model.h>
#include <memory>
#include <component.h>

namespace GW {
	Entity::Entity()
	{
		m_world = nullptr;
		rootComponent = std::make_unique<GW::Component>();
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