#pragma once

#include <string>
#include <vector>


typedef float GLfloat;
typedef unsigned char GLubyte;
namespace GW {
	class World;
	namespace RenderEngine {
		//structs used for model rendering
		struct Position {
			GLfloat x;
			GLfloat y;
		};

		struct Color {
			GLubyte r;
			GLubyte g;
			GLubyte b;
		};

		struct Vertex {
			Position position;
			Color color;
		};


		class Model {
		public:
			Model();
			~Model();

			//loads a model from a file to be used when rendered
			void loadFromFile(std::string fileName);
		private:
			//deals with drawing the files during rendering
			void draw();

			friend class GW::World;
		};
	}
}