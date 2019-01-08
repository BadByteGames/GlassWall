#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace GW {
	namespace RenderEngine {
		struct Light {
			Light(){}
			Light(glm::vec3 color, glm::vec3 position, float specularStrength, float ambientStrength):color(color),
				position(position),
				specularStrength(specularStrength),
				ambientStrength(ambientStrength){}

			glm::vec3 color;
			glm::vec3 position;
			float specularStrength;
			float ambientStrength;
		};

		struct DirectionalLight
		{
			DirectionalLight() {}
			DirectionalLight(glm::vec3 color, glm::vec3 direction, float specularStrength, float ambientStrength) :color(color),
				direction(direction),
				specularStrength(specularStrength),
				ambientStrength(ambientStrength) {}
			glm::vec3 color;
			glm::vec3 direction;
			float specularStrength;
			float ambientStrength;
		};

		class Lighting {
		public:
			Lighting();
			~Lighting();

			void setDirectionalLight(const DirectionalLight& light);

			void addLight(const Light& light);
			
			std::vector<Light> getLights();

			DirectionalLight getDirectionalLight();
		private:
			DirectionalLight m_directionalLight;
			std::vector<Light> m_lights;
		};
	}
}