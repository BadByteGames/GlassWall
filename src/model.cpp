#include <model.h>
#include <shaderprogram.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <world.h>
#include <camera.h>
#include <textures.h>
#include <string>
#include <colladaloader.h>
#include <lighting.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

GW::RenderEngine::Model::Model()
{
	Component();
	m_type = "model";
	m_program = 0;
	m_vbo = 0;
}

GW::RenderEngine::Model::~Model()
{
}

void GW::RenderEngine::Model::loadFromFile(std::string fileName)
{
	ColladaLoader colladaLoader;
	colladaLoader.loadFromFile(fileName);

	m_vertices = colladaLoader.getVertices();

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

void GW::RenderEngine::Model::useShader(const ShaderProgram & shader)
{
	//set shader to use to shader id
	m_program = shader.getProgram();
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
	GLint uvAttrib = glGetAttribLocation(m_program, "in_uv");
	GLint normalAttrib = glGetAttribLocation(m_program, "in_normal");

	//get program uniforms
	GLint modelUniform = glGetUniformLocation(m_program, "model");
	GLint viewUniform = glGetUniformLocation(m_program, "view");
	GLint projectionUniform = glGetUniformLocation(m_program, "projection");
	GLint mvpUniform = glGetUniformLocation(m_program, "mvp");
	GLint ambientStrengthUniform = glGetUniformLocation(m_program, "ambientstrength");
	GLint lightPosUniform = glGetUniformLocation(m_program, "lightpos");
	GLint lightColorUniform = glGetUniformLocation(m_program, "lightcolor");
	GLint invertedModelUniform = glGetUniformLocation(m_program, "invertedmodel");
	GLint viewerPosUniform = glGetUniformLocation(m_program, "viewerpos");
	GLint specularStrengthUniform = glGetUniformLocation(m_program, "specularstrength");

	//enable attributes as needed
	if (positionAttrib != -1) {
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	}

	if (colorAttrib != -1) {
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	}

	if (uvAttrib != -1) {
		glEnableVertexAttribArray(uvAttrib);
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (normalAttrib != -1) {
		glEnableVertexAttribArray(normalAttrib);
		glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	}

	//set uniforms to proper values
	if (modelUniform != -1) {
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(getTransform()));
	}

	if (viewUniform != -1) {
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getViewMatrix()));
	}

	if (projectionUniform != -1) {
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getProjectionMatrix()));
	}

	if (mvpUniform != -1) {
		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getProjectionMatrix() * m_world->getCamera()->getViewMatrix() * getTransform()));
	}

	if (invertedModelUniform != -1) {
		glUniformMatrix4fv(invertedModelUniform, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(getTransform()))));
	}

	LightColor lightColor = m_world->getLighting()->getLights()[0].color;
	if (lightColorUniform != -1) {
		glUniform3fv(lightColorUniform, 1, glm::value_ptr(glm::vec3(lightColor.r, lightColor.g, lightColor.b)));
	}

	Position lightPos = m_world->getLighting()->getLights()[0].position;
	if (lightPosUniform != -1) {
		glUniform3fv(lightPosUniform, 1, glm::value_ptr(glm::vec3(lightPos.x, lightPos.y, lightPos.z)));
	}

	if (specularStrengthUniform != -1) {
		glUniform1fv(specularStrengthUniform, 1, &m_world->getLighting()->getLights()[0].specularStrength);
	}

	if (ambientStrengthUniform != -1) {
		glUniform1fv(ambientStrengthUniform, 1, &m_world->getLighting()->getLights()[0].ambientStrength);
	}

	if (viewerPosUniform != -1) {
		glUniform3fv(viewerPosUniform, 1, glm::value_ptr(m_world->getCamera()->getPosition()));
	}

	//set all sampler2D values
	for (unsigned int i = 0; i < 16; i++) {
		GLint samplerLocation = glGetUniformLocation(m_program, std::string("textureSlot" + std::to_string(i)).c_str());
		if (samplerLocation != -1) {
			glUniform1i(samplerLocation, i);
		}
	}

	//draw shapes
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());

	//disable attributes as needed
	if (positionAttrib != -1) {
		glDisableVertexAttribArray(positionAttrib);
	}

	if (colorAttrib != -1) {
		glDisableVertexAttribArray(colorAttrib);
	}

	if (uvAttrib != -1) {
		glDisableVertexAttribArray(uvAttrib);
	}

	if (normalAttrib != -1) {
		glDisableVertexAttribArray(normalAttrib);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//stop using shader
	glUseProgram(0);
}
