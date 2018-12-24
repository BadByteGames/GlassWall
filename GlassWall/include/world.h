#pragma once

#include <vector>
#include <settings.h>

namespace GW {
	namespace RenderEngine{
		class Window;
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

	private:
		void update();

		std::vector<Entity*> m_entities;

		bool m_requestQuit;
		bool m_worldStarted;

		//window of game
		RenderEngine::Window* m_window;
	};
}