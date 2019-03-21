#include "shaderprogram.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <util.h>

//default shader source to use including enough to render textures
//no advanced effects
const char* defaultVert = R"(#version 130)";

const char* defaultFrag = R"(#version 130)";

namespace GW {
	namespace RenderEngine {
		ShaderProgram::ShaderProgram()
		{
			m_vertexShader = 0;
			m_fragmentShader = 0;
			m_program = 0;
		}

		ShaderProgram::~ShaderProgram()
		{
		}

		int ShaderProgram::compileShadersFromFile(std::string vertexShader, std::string fragmentShader)
		{
			//compile the shaders
			compileShader(vertexShader, m_vertexShader, GL_VERTEX_SHADER);
			compileShader(fragmentShader, m_fragmentShader, GL_FRAGMENT_SHADER);

			//create the program
			m_program = glCreateProgram();
			glAttachShader(m_program, m_vertexShader);
			glAttachShader(m_program, m_fragmentShader);

			glLinkProgram(m_program);

			//check for link errors
			GLint isLinked = 0;
			glGetProgramiv(m_program, GL_LINK_STATUS, (int *)&isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint log_length;
				glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);

				std::vector<char> v(log_length);
				glGetProgramInfoLog(m_program, log_length, NULL, v.data());
				std::string s(begin(v), end(v));

				// We don't need the program anymore.
				glDeleteProgram(m_program);
				// Don't leak shaders either.
				glDeleteShader(m_vertexShader);
				glDeleteShader(m_fragmentShader);

				// Use the infoLog as you see fit.
				std::cout << "Program link failed: " << s << std::endl;
				// In this simple program, we'll just leave
				return 1;
			}

			return 0;
		}

		void ShaderProgram::destroy()
		{
			glDeleteProgram(m_program);
		}

		int ShaderProgram::compileShader(std::string shaderFile, GLuint& shader, GLenum type)
		{
			//create shader
			shader = glCreateShader(type);

			std::string source = Util::getFileContents(shaderFile);
			const char* tempCstr = source.c_str();
			GLint length = static_cast<GLint>(source.size());
			glShaderSource(shader, 1, &tempCstr, &length);

			glCompileShader(shader);

			GLint success = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (success == GL_FALSE)
			{
				GLint log_length;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

				std::vector<char> v(log_length);
				glGetShaderInfoLog(shader, log_length, NULL, v.data());
				std::string s(begin(v), end(v));


				std::cout << "Failed to compile " << shaderFile << ":\n" << s << std::endl;

				// Provide the infolog in whatever manor you deem best.
				// Exit with failure.
				glDeleteShader(shader); // Don't leak the shader.
				return 1;
			}

			return 0;
		}

	}
}