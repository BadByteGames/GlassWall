#include <world.h>

#include <iostream>
#include <entity.h>
#include <renderengine.h>
#include <window.h>
#include <SDL.h>

namespace GW {
	World::World() : m_requestQuit(false), m_worldStarted(false)
	{
	}

	World::~World()
	{
	}

	void World::start()
	{
		//initialize rendering
		RenderEngine::init();

		//create a window
		m_window = new RenderEngine::Window();
		m_window->create("Filler Text", 640, 480, 0);

		//init glew
		RenderEngine::initGL();

		//trigger all the entity world start events
		for (auto ent : m_entities) {
			ent->onWorldStart();
		}

		//trigger any entity start events
		for (auto ent : m_entities) {
			ent->entityStart();
		}

		while (!m_requestQuit) {
			//clear the window
			m_window->clear(1.0f, 0.0f, 0.0f);

			//update the world until stopped
			update();

			//quick hack for handling SDL Input
			//TODO: Replace with actual input system
			SDL_Event evnt;
			while (SDL_PollEvent(&evnt)) {
				if (evnt.type == SDL_QUIT) {
					m_requestQuit = true;
				}
			}

			//swap buffers
			m_window->swapBuffers();
		}

		//call entity cleanup events
		for (auto ent : m_entities) {
			ent->cleanUp();
		}

		//cleanup renderengine
		m_window->destroy();
		delete m_window;

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