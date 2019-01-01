#include <model.h>
#include <shaderprogram.h>
#include <iostream>

GW::RenderEngine::Model::Model()
{
	m_program = 0;
	m_vbo = 0;
}

GW::RenderEngine::Model::~Model()
{
}

void GW::RenderEngine::Model::loadFromFile(std::string fileName)
{

}

void GW::RenderEngine::Model::useShader(const ShaderProgram & shader)
{
	//set shader to use to shader id
	m_program = shader.getProgram();
}

void GW::RenderEngine::Model::test()
{

	//fill up the model with some test data
	m_vertices.push_back({ {0.5f, 0.5f, 5.0f}, {255, 0, 0} });
	m_vertices.push_back({ { 0.5f, 0.0f, 5.0f }, {255, 0, 0, 255 }});
	m_vertices.push_back({ { 0.0f, 0.0f, 5.0f }, {255, 0, 0, 255 }});

	//generate a vbo
	if (m_vbo == 0) {
		glGenBuffers(1, &m_vbo);
	}

	//set vbo to active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//set data
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GW::RenderEngine::Model::cleanUp()
{
	if (m_vbo != 0) {
		glDeleteBuffers(1, &m_vbo);
	}
}

void GW::RenderEngine::Model::draw()
{
	//start using shader
	glUseProgram(m_program);

	//get all attribs
	GLint positionAttrib = glGetAttribLocation(m_program, "in_position");
	GLint colorAttrib = glGetAttribLocation(m_program, "in_color");
	
	//stop using shader
	glUseProgram(0);
}
