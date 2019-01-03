#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

namespace GW {
	class World;
	namespace RenderEngine {
		class ShaderProgram;

		//structs used for model rendering
		struct Uv {
			Uv() : u(0.0f), v(0.0f) {}
			Uv(GLfloat u, GLfloat v) : u(u), v(v) {}
			GLfloat u;
			GLfloat v;
		};
		

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

		//define some ease of use colors
		const Color WHITE(255, 255, 255);
		const Color BLACK(0, 0, 0);

		struct Vertex {
			Vertex() {}
			Vertex(Position pos, Color color, Uv uv) : position(pos), color(color), uv(uv) {}
			Position position;
			Color color;
			Uv uv;
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

			//set world rules
			void setWorld(GW::World* world);
		private:
			//deals with drawing the files during rendering
			void draw();

			//stores all vertex data
			std::vector<Vertex> m_vertices;

			//shader id
			GLuint m_program;

			//vbo id
			GLuint m_vbo;

			//refrence to world
			GW::World* m_world;

			friend class GW::World;
		};
	}
}