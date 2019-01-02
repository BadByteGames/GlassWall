#pragma once

#include <glm/glm.hpp>

namespace GW{
	namespace RenderEngine{
		class Camera {
		public:
			Camera();
			~Camera();

			void setAbsolutePosition(const glm::vec3& position);

		private:
			glm::vec3 m_position;
		};
	}
}