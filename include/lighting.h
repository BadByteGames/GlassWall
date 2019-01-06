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
			Light(LightColor color, Position position, float specularStrength, float ambientStrength):color(color),
				position(position),
				specularStrength(specularStrength),
				ambientStrength(ambientStrength){}

			LightColor color;
			Position position;
			float specularStrength;
			float ambientStrength;
		};

		class Lighting {
		public:
			Lighting();
			~Lighting();

			void addLight(const Light& light);
			
			std::vector<Light> getLights();

		private:
			std::vector<Light> m_lights;
		};
	}
}