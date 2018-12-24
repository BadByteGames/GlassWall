#pragma once

#include <string>

namespace GW {
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

		//name of entity
		std::string m_name;
	};
}