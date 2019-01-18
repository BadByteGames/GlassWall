#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace GW {
	class Component;
	class World;

	class Component {
	public:
		Component();
		~Component();

		virtual void update();

		virtual void draw();

		void addChild(Component* component);

		void setAbsolutePosition(glm::vec3 position);
	
		
		void setRelativePosition(glm::vec3 position);

		//angles in degrees
		void setAbsoluteOrientation(glm::vec3 angles);

		//angles in degrees
		void setRelativeOrientation(glm::vec3 angles);

		//get a transform matrix
		glm::mat4 getTransform();

		//get children
		std::vector<Component*> getChildren();

		Component* getParent();

		//get position
		glm::vec3 getAbsolutePosition();
		glm::vec3 getRelativePosition();
	private:
		friend class GW::World;

		//stores position in world
		glm::vec3 m_absolutePosition;

		//stores position relative to parent
		glm::vec3 m_relativePosition;

		//stores absolute angle measures
		glm::vec3 m_absoluteOrientation;

		//stores relative orientation
		glm::vec3 m_relativeOrientation;
			
		//the parent of this component
		Component* m_parent;

		//children of this component
		std::vector<Component*> m_children;

		//stores wheter the last position update was absolute
		bool m_absoluteMode;

	protected:
		GW::World * m_world;
		std::string m_type;
	};
}