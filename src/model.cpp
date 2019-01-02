#include <model.h>
#include <shaderprogram.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <world.h>
#include <camera.h>

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
	m_vertices.push_back({ {0.5f, 0.5f, -1.0f}, {255, 0, 0} });
	m_vertices.push_back({ { 0.5f, -0.5f, -1.0f }, {255, 0, 0 }});
	m_vertices.push_back({ { -0.5f, -0.5f, -1.0f }, {255, 0, 0 }});
	m_vertices.push_back({ {0.5f, 0.5f, -1.0f}, {255, 0, 0} });
	m_vertices.push_back({ { -0.5f, 0.5f, -1.0f }, {255, 0, 0 } });
	m_vertices.push_back({ { -0.5f, -0.5f, -1.0f }, {255, 0, 0 } });

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

void GW::RenderEngine::Model::setWorld(GW::World * world)
{
	m_world = world;
}

void GW::RenderEngine::Model::draw()
{
	//start using shader
	glUseProgram(m_program);

	//set the active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//get all attribs
	GLint positionAttrib = glGetAttribLocation(m_program, "in_position");
	GLint colorAttrib = glGetAttribLocation(m_program, "in_color");
	
	//get program uniforms
	GLint modelUniform = glGetUniformLocation(m_program, "model");
	GLint viewUniform = glGetUniformLocation(m_program, "view");
	GLint projectionUniform = glGetUniformLocation(m_program, "projection");
	GLint mvpUniform = glGetUniformLocation(m_program, "mvp");

	//enable attributes as needed
	if (positionAttrib != -1) {
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	}

	if (positionAttrib != -1) {
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	}

	//set uniforms to proper values
	if (modelUniform != -1) {
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	}

	if (viewUniform != -1) {
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getViewMatrix()));
	}

	if (projectionUniform != -1) {
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getProjectionMatrix()));
	}

	if (mvpUniform != -1) {
		//glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getViewMatrix() * m_world->getCamera()->getProjectionMatrix()));
	}
	//draw shapes
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

	//disable attributes as needed
	if (positionAttrib != -1) {
		glDisableVertexAttribArray(positionAttrib);
	}

	if (colorAttrib != -1) {
		glDisableVertexAttribArray(colorAttrib);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//stop using shader
	glUseProgram(0);
}
