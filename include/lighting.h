#pragma once

#include <glm/glm.hpp>
#include <vertex.h>
#include <vector>

namespace GW {
	namespace RenderEngine {
		struct LightColor {
			LightColor() : r(0.0f), g(0.0f), b(0.0f) {}
			LightColor(GLfloat r, GLfloat g, GLfloat b) : r(r), g(g), b(b) {}
			GLfloat r;
			GLfloat g;
			GLfloat b;
		};

		struct Light {
			Light(){}
			Light(LightColor color, Position position, float strength):color(color), position(position), specularStrength(strength){}

			LightColor color;
			Position position;
			float specularStrength;
		};

		class Lighting {
		public:
			Lighting();
			~Lighting();

			void setAmbient(const glm::vec3& ambientColor, const float& ambientStrength);
			void setAmbientStrength(const float& ambientStrength);
			void setAmbientColor(const glm::vec3& ambientColor);

			float getAmbientStrength();
			glm::vec3 getAmbientColor();

			void addLight(const Light& light);
			
			std::vector<Light> getLights();

		private:
			float m_ambientStrength;
			glm::vec3 m_ambientColor;
			std::vector<Light> m_lights;
		};
	}
}