#pragma once

#include <string>

namespace GW {
	namespace RenderEngine {
		class Model;
	}

	class Entity {
	public:
		Entity(std::string);
		~Entity();

		virtual void entityStart();

		virtual void onWorldStart();

		virtual void update();

		virtual void cleanUp();

	private:
		friend class World;

	protected:
		//refrence to parent world
		World* m_world;
		RenderEngine::Model* m_model;

		//name of entity
		std::string m_name;
	};
}