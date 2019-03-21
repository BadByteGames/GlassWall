#pragma once

#include <string>
#include <memory>

namespace GW {
	namespace RenderEngine {
		class Model;
	}
	
	class Component;
	class Entity {
	public:
		Entity();
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

		std::unique_ptr<Component> rootComponent;
	};
}