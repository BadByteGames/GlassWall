#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace GW {
	namespace RenderEngine {
		struct PointLight {
			PointLight(){}
			PointLight(glm::vec3 color, 
				glm::vec3 position, 
				float specularStrength, 
				float ambientStrength, 
				float constant, 
				float linear, 
				float quadratic):color(color),
				position(position),
				specularStrength(specularStrength),
				ambientStrength(ambientStrength), 
				constant(constant), 
				linear(linear), 
				quadratic(quadratic){}

			glm::vec3 color;
			glm::vec3 position;
			float specularStrength;
			float ambientStrength;

			float constant;
			float linear;
			float quadratic;
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

			void addPointLight(const PointLight& light);
			
			void setPointLight(unsigned int index, const PointLight& light);
			
			std::vector<PointLight> getPointLights();

			DirectionalLight getDirectionalLight();
		private:
			DirectionalLight m_directionalLight;
			std::vector<PointLight> m_pointLights;
		};
	}
}