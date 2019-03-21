#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <vertex.h>
#include <component.h>
#include <textures.h>

namespace GW {
	class World;
	namespace RenderEngine {
		class ShaderProgram;

		class Model : public Component {
		public:
			Model();
			~Model();

			//loads a model from a file to be used when rendered from a .dae
			void loadFromFile(std::string fileName);

			//sets the model to a box with the matching dimensions
			void setBox(glm::vec3 dims);

			//what shader the model should use
			void useShader(const ShaderProgram& shader);
			void useShader(const unsigned int& shaderID);

			//clean up internals
			void cleanUp();

			//set world rules
			void setWorld(GW::World* world);

			//set the material
			void setMaterial(Material material);

			//deals with drawing the files during rendering
			virtual void draw();
		private:
			//stores all vertex data
			std::vector<Vertex> m_vertices;

			//shader id
			GLuint m_program;

			//vbo id
			GLuint m_vbo;

			//material of model
			GW::RenderEngine::Material m_material;

			friend class GW::World;
		};
	}
}