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

		struct SpotLight {
			SpotLight(){}
			SpotLight(glm::vec3 color,
				glm::vec3 position,
				glm::vec3 direction,
				float specularStrength,
				float ambientStrength,
				float cutoff, //<<cutoff angle in degrees
				float outerCutoff, //<<outer cutoff angle in degrees
				float constant,
				float linear,
				float quadratic
			) : color(color),
				position(position),
				direction(direction),
				ambientStrength(ambientStrength),
				specularStrength(specularStrength),
				cutoff(cutoff),
				outerCutoff(outerCutoff),
				constant(constant),
				linear(linear),
				quadratic(quadratic)
				{}

			glm::vec3 color;
			glm::vec3 position;
			glm::vec3 direction;

			float specularStrength;
			float ambientStrength;

			//in degrees
			float cutoff;
			float outerCutoff;			

			float constant;
			float linear;
			float quadratic;
		};

		struct DirectionalLight
		{
			DirectionalLight() { color = glm::vec3(0.0f); }
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
			
			void addSpotLight(const SpotLight& light);

			void setPointLight(unsigned int index, const PointLight& light);
			
			void setSpotLight(unsigned int index, const SpotLight& light);

			PointLight getPointLight(unsigned int index);

			std::vector<PointLight> getPointLights();
			std::vector<SpotLight> getSpotLights();

			DirectionalLight getDirectionalLight();
		private:
			DirectionalLight m_directionalLight;
			std::vector<PointLight> m_pointLights;
			std::vector<SpotLight> m_spotLights;
		};
	}
}