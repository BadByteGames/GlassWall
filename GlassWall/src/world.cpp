#include <world.h>

#include <iostream>
#include <entity.h>

namespace GW {
	World::World() : m_requestQuit(false), m_worldStarted(false)
	{
	}

	World::~World()
	{
	}

	void World::start()
	{
		//trigger all the entity world start events
		for (auto ent : m_entities) {
			ent->onWorldStart();
		}

		//trigger any entity start events
		for (auto ent : m_entities) {
			ent->entityStart();
		}

		while (!m_requestQuit) {
			//update the world until stopped
			update();
		}

		//call entity cleanup events
		for (auto ent : m_entities) {
			ent->cleanUp();
		}

		//don't delete entities managed by user
	}

	void World::addEntity(Entity * entity)
	{
		entity->m_world = this;
		m_entities.push_back(entity);

		//init entity if world already started
		entity->entityStart();
	}

	void World::update()
	{
		//call all entity update functions
		for (auto ent : m_entities) {
			ent->update();
		}
	}
}