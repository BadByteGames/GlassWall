#pragma once

#include <string>

namespace GW {
	namespace RenderEngine {
		class Model;
	}
	
	class Component;
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

		Component* rootComponent;

		//name of entity
		std::string m_name;
	};
}