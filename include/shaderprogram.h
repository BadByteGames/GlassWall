#pragma once

#include <string>
#include <GL/glew.h>

namespace GW {
	namespace RenderEngine {
		class ShaderProgram {
		public:
			ShaderProgram();
			~ShaderProgram();

			int compileShadersFromFile(std::string vertexShader, std::string fragmentShader);

			GLuint getProgram() const { return m_program; }

			void destroy();

		private:
			std::string getFileContents(std::string fileName);

			int compileShader(std::string shaderFile, GLuint& shader, GLenum type);


			GLuint m_vertexShader;
			GLuint m_fragmentShader;

			GLuint m_program;
		};
	}
}