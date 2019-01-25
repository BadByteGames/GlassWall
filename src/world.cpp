#include <world.h>

#include <iostream>
#include <entity.h>
#include <renderengine.h>
#include <window.h>
#include <SDL.h>
#include <model.h>
#include <camera.h>
#include <textures.h>
#include <component.h>
#include <lighting.h>

namespace GW {
	World::World() : m_requestQuit(false), m_worldStarted(false), m_windowFlags(0)
	{
		m_camera = new RenderEngine::Camera();
		m_lighting = new RenderEngine::Lighting();
		m_window = new RenderEngine::Window();
	}

	World::~World()
	{
	}

	void World::start()
	{
		//initialize rendering
		RenderEngine::init();

		//create a window
		m_window->create("Glass Wall", 1280, 720, m_windowFlags);

		//init glew
		RenderEngine::initGL();

		m_camera->setDimensions(1280, 720);
		m_inputManager.setWindowDimensions(1280, 720);

		//trigger all the entity world start events
		for (auto ent : m_entities) {
			ent->onWorldStart();
		}

		m_worldStarted = true;

		//trigger any entity start events
		for (auto ent : m_entities) {
			ent->entityStart();
			initComponent(ent->rootComponent);
		}

		while (!m_requestQuit) {
			m_fpsCounter.startFrame();
			//clear the window to black
			m_window->clear(0.0f, 0.0f, 0.0f);

			//update input
			m_inputManager.update();

			//update the world until stopped
			update();

			//quit if necessary
			if (m_inputManager.quitRequested()) {
				m_requestQuit = true;
			}
						
			//draw entity models
			draw();

			//swap buffers
			m_window->swapBuffers();
			m_fpsCounter.endFrame();

			//set window title
			m_window->setTitle("Glass Wall | "+std::to_string(m_fpsCounter.getFps())+" FPS");
		}

		//call entity cleanup events
		for (auto ent : m_entities) {
			ent->cleanUp();
		}

		//cleanup renderengine
		m_window->destroy();
		delete m_window;
		delete m_camera;
		delete m_lighting;
		
		RenderEngine::Textures::clearCache();

		//don't delete entities managed by user
	}

	void World::addEntity(Entity * entity)
	{
		entity->m_world = this;
		m_entities.push_back(entity);

		//init entity if world already started
		if (m_worldStarted) {
			entity->entityStart();
			initComponent(entity->rootComponent);
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

	InputManager * World::getInputManager()
	{
		return &m_inputManager;
	}

	RenderEngine::Lighting * World::getLighting()
	{
		return m_lighting;
	}

	FpsCounter * World::getFpsCounter()
	{
		return &m_fpsCounter;
	}

	bool World::getWorldStarted()
	{
		return m_worldStarted;
	}

	void World::update()
	{
		//call all entity update functions
		for (auto ent : m_entities) {
			ent->update();
			updateComponent(ent->rootComponent);
		}
	}
	void World::draw()
	{
		for (auto it : m_entities) {
			drawComponent(it->rootComponent);
		}
	}

	void World::initComponent(Component * component)
	{
		if (component != nullptr) {
			component->m_world = this;
			for (auto it : component->getChildren()) {
				initComponent(it);
			}
		}
	}

	void World::updateComponent(Component * component)
	{
		if (component != nullptr) {
			component->update();
			for (auto it : component->getChildren()) {
				updateComponent(it);
			}
		}
	}

	void World::drawComponent(Component * component)
	{	
		if (component != nullptr) {
			component->draw();
			for (auto it : component->getChildren()) {
				drawComponent(it);
			}
		}
	}
}