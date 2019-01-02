#pragma once

#include <glm/glm.hpp>

namespace GW{
	namespace RenderEngine{
		class Camera {
		public:
			Camera();
			~Camera();
			
			//sets absolute position in world
			void setAbsolutePosition(const glm::vec3& position);
			
			//set rotation values (in degrees)
			void setRotation(const glm::vec3& rotations);

			//set rotation values (in degrees)
			void setRotation(float x, float y, float z);

			//sets target
			void setTarget(const glm::vec3& target);

			//sets up vector
			void setUpVector(const glm::vec3& up);

			/*sets camera ortho value
			*true = orthographic
			*false = perspective
			*/
			void setOrthopgraphic(const bool& value);

			//should target not be calculated every frame
			void useTarget(const bool& value);

			//set window dimensions
			void setDimensions(const int& width, const int& height);

			//calculates necessary matrices for graphics
			glm::mat4 getViewMatrix();
			glm::mat4 getProjectionMatrix();

			//sets the field of viw
			void setFOV(const float& fov);
		private:
			//stores camera position
			glm::vec3 m_position;
			//stores the location for the camera to look at
			glm::vec3 m_target;
			//stores the "up" direction of the camera
			glm::vec3 m_up;
			//stores x, y, and z rotations stored in radians
			glm::vec3 m_rotations;

			//is camera orthographic
			bool m_orthographic;

			//is camera in look at mode
			bool m_lookat;

			//store width and height for perspective calculations
			int m_width;
			int m_height;

			//field of vision
			float m_fov;
		};
	}
}