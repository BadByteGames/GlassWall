#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

namespace GW {
	class World;
	namespace RenderEngine {
		class ShaderProgram;

		//structs used for model rendering
		struct Position {
			Position(): x(0.0f), y(0.0f), z(0.0f) {}
			Position(GLfloat x, GLfloat y, GLfloat z): x(x), y(y), z(z) {}
			GLfloat x;
			GLfloat y;
			GLfloat z;
		};

		struct Color {
			Color(): r(0), g(0), b(0), a(0) {}
			Color(GLubyte r, GLubyte g, GLubyte b) : r(r), g(g), b(b), a(255) {}
			Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a): r(r), g(g), b(b), a(a) {}
			GLubyte r;
			GLubyte g;
			GLubyte b;
			GLubyte a;
		};

		struct Vertex {
			Vertex() {}
			Vertex(Position pos, Color color) : position(pos), color(color) {}
			Position position;
			Color color;
		};


		class Model {
		public:
			Model();
			~Model();

			//loads a model from a file to be used when rendered
			void loadFromFile(std::string fileName);

			//what shader the model should use
			void useShader(const ShaderProgram& shader);

			//temporary test draw function to check rendering system
			//remove when model loading is in place
			void test();

			//clean up internals
			void cleanUp();
		private:
			//deals with drawing the files during rendering
			void draw();

			//stores all vertex data
			std::vector<Vertex> m_vertices;

			//shader id
			GLuint m_program;

			//vbo id
			GLuint m_vbo;

			friend class GW::World;
		};
	}
}