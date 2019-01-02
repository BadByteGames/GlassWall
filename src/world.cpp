#include <world.h>

#include <iostream>
#include <entity.h>
#include <renderengine.h>
#include <window.h>
#include <SDL.h>
#include <model.h>
#include <camera.h>

namespace GW {
	World::World() : m_requestQuit(false), m_worldStarted(false), m_windowFlags(0)
	{
		m_camera = new RenderEngine::Camera();
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
		m_window->create("Filler Text", 1280, 720, m_windowFlags);

		//init glew
		RenderEngine::initGL();

		m_camera->setTargetLock(true);
		m_camera->setDimensions(1280, 720);

		//trigger all the entity world start events
		for (auto ent : m_entities) {
			ent->onWorldStart();
		}

		//trigger any entity start events
		for (auto ent : m_entities) {
			ent->entityStart();
			if (ent->m_model != nullptr) {
				ent->m_model->setWorld(this);
			}
		}

		m_worldStarted = true;

		while (!m_requestQuit) {
			//clear the window to black
			m_window->clear(0.0f, 0.0f, 0.0f);

			//update the world until stopped
			update();

			m_camera->update();

			//quick hack for handling SDL Input
			//TODO: Replace with actual input system
			SDL_Event evnt;
			while (SDL_PollEvent(&evnt)) {
				if (evnt.type == SDL_QUIT) {
					m_requestQuit = true;
				}
			}

			//draw entity models
			for (auto ent : m_entities) {
				ent->m_model->draw();
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
		delete m_camera;

		//don't delete entities managed by user
	}

	void World::addEntity(Entity * entity)
	{
		entity->m_world = this;
		m_entities.push_back(entity);

		//init entity if world already started
		if (m_worldStarted) {
			entity->m_model->setWorld(this);
			entity->entityStart();
		}
	}

	void World::setWindowFullscreen(bool value)
	{
		if (m_worldStarted) {
			m_window->setFullscreen(value);
		}
		else {
			m_windowFlags ^= (-(value ? 1 : 0)) & WINDOWFLAGS::FULLSCREEN;
		}
	}

	void World::setWindowBorderless(bool value)
	{
		m_window->setBorderless(value);
	}

	void World::setWindowResizable(bool value)
	{
		m_window->setResizable(value);
	}

	void World::setWindowFlags(unsigned int value)
	{
		m_windowFlags = value;
	}

	RenderEngine::Camera * World::getCamera()
	{
		return m_camera;
	}

	void World::update()
	{
		//call all entity update functions
		for (auto ent : m_entities) {
			ent->update();
			if (ent->m_model != nullptr) {
				ent->m_model->setWorld(this);
			}
		}
	}
}