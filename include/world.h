#pragma once

#include <vector>
#include <settings.h>

namespace GW {
	namespace RenderEngine{
		class Window;
		class Camera;
	}

	class Entity;

	class World {
	public:
		World();
		~World();

		/*
		*Starts the world after everything has been added
		*/
		void start();

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
	private:
		void update();

		std::vector<Entity*> m_entities;

		unsigned int m_windowFlags;

		bool m_requestQuit;
		bool m_worldStarted;

		//window of game
		RenderEngine::Window* m_window;

		//currently active camera
		RenderEngine::Camera* m_camera;
	};
}