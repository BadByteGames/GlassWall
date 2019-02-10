#pragma once

#include <vector>
#include <settings.h>
#include <inputmanager.h>
#include <fpscounter.h>
#include <lighting.h>
#include <camera.h>
#include <memory>

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
		*Creates an entity object in the world
		*/
		void addEntity(Entity* entity);

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

		std::vector<Entity*> m_entities;

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