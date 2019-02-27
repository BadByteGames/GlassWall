#pragma once

#include <vector>
#include <settings.h>
#include <inputmanager.h>
#include <fpscounter.h>
#include <lighting.h>
#include <camera.h>
#include <memory>
#include <functional>

namespace GW {
	class Entity;
	class Component;

	namespace RenderEngine {
		class Window;
	}

	class World {
	public:
		World();
		~World();

		/*
		*Starts the world after everything has been added
		*/
		void start(std::string configName);

		/*
		*Adds a lambda that spawns an entity of that name
		*/
		void addEntitySpawner(std::string name, std::function<std::unique_ptr<Entity>()> entitySpawner);

		/*
		*Creates a new entity using a lambda
		*/
		std::unique_ptr<Entity> generateEntity(std::string name);

		/*
		*adds an entity into the world
		*/
		void addEntity(std::unique_ptr<Entity> entity);

		/*
		*Ask the world to stop
		*/
		void requestQuit() { m_requestQuit = true; }

		//set the window to fullscreen
		void setWindowFullscreen(bool value);

		//set the window to be borderless
		void setWindowBorderless(bool value);

		//set the window to resize
		void setWindowResizable(bool value);

		//set window flags PRIOR to start()
		void setWindowFlags(unsigned int value);

		//get a reference to the camera
		RenderEngine::Camera* getCamera();

		InputManager* getInputManager();

		RenderEngine::Lighting* getLighting();

		FpsCounter* getFpsCounter();

		//has the world started
		bool getWorldStarted();

		//will load the level from this file
		void loadFromFile(const std::string& fileName);
	private:
		void update();

		void draw();

		void initComponent(Component* component);

		void updateComponent(Component* component);

		void drawComponent(Component* component);

		std::vector<std::unique_ptr<Entity>> m_entities;

		//a map of entity spawners used to create entities
		std::unordered_map<std::string, std::function<std::unique_ptr<Entity>()>> m_spawners;

		unsigned int m_windowFlags;

		bool m_requestQuit;
		bool m_worldStarted;

		//window of game
		std::unique_ptr<RenderEngine::Window> m_window;

		//currently active camera
		std::unique_ptr<RenderEngine::Camera> m_camera;

		//stores all lighting
		std::unique_ptr<RenderEngine::Lighting> m_lighting;

		//stores key values
		std::unique_ptr<InputManager> m_inputManager;

		//calculate FPS
		std::unique_ptr<FpsCounter> m_fpsCounter;
	};
}