#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace GW {
	namespace RenderEngine {
		//structs used for model rendering
		struct Uv {
			Uv() : u(0.0f), v(0.0f) {}
			Uv(glm::vec2 uv) : u(uv.x), v(uv.y) {}
			Uv(GLfloat u, GLfloat v) : u(u), v(v) {}
			GLfloat u;
			GLfloat v;
		};

		struct Position {
			Position() : x(0.0f), y(0.0f), z(0.0f) {}
			Position(glm::vec3 pos) : x(pos.x), y(pos.y), z(pos.z) {}
			Position(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
			GLfloat x;
			GLfloat y;
			GLfloat z;
		};

		struct Normal {
			Normal() : x(0.0f), y(0.0f), z(0.0f) {}
			Normal(glm::vec3 normal) : x(normal.x), y(normal.y), z(normal.z) {}
			Normal(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
			GLfloat x;
			GLfloat y;
			GLfloat z;
		};

		struct Color {
			Color() : r(255), g(255), b(255), a(255) {}
			Color(GLubyte r, GLubyte g, GLubyte b) : r(r), g(g), b(b), a(255) {}
			Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) {}
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
			Vertex(Position pos, Color color, Uv uv, Normal normal) : position(pos), color(color), uv(uv), normal(normal) {}
			Position position;
			Color color;
			Uv uv;
			Normal normal;
		};
	}
}