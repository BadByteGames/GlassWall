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
#include <textures.h>
#include <lighting.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

GW::RenderEngine::Model::Model()
{
	Component();
	m_type = "model";
	m_program = 0;
	m_material = Material(0, 0);
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

void GW::RenderEngine::Model::setMaterial(Material material)
{
	m_material = material;
}

void GW::RenderEngine::Model::draw()
{
	//start using shader
	glUseProgram(m_program);

	//set the active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//override the texture for slot 0 and 1
	Textures::setTextureSlot(m_material.diffuseID, 0);
	Textures::setTextureSlot(m_material.specularID, 1);

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
	GLint invertedModelUniform = glGetUniformLocation(m_program, "invertedmodel");
	GLint viewerPosUniform = glGetUniformLocation(m_program, "viewerpos");

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

	glm::mat4 modelTransform = getTransform();

	//set uniforms to proper values
	if (modelUniform != -1) {
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelTransform));
	}

	if (viewUniform != -1) {
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getViewMatrix()));
	}

	if (projectionUniform != -1) {
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getProjectionMatrix()));
	}

	if (mvpUniform != -1) {
		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(m_world->getCamera()->getProjectionMatrix() * m_world->getCamera()->getViewMatrix() * modelTransform));
	}

	if (invertedModelUniform != -1) {
		glUniformMatrix4fv(invertedModelUniform, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelTransform))));
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

	std::vector<PointLight> pointLights = m_world->getLighting()->getPointLights();
	//set all light uniforms

	//set directional light
	DirectionalLight directional= m_world->getLighting()->getDirectionalLight();
	GLint directionalAmbientStrengthUniform = glGetUniformLocation(m_program, std::string("directionallight.ambientstrength").c_str());
	if (directionalAmbientStrengthUniform != -1) {
		glUniform1fv(directionalAmbientStrengthUniform, 1, &directional.ambientStrength);
	}

	GLint derectionalSpecularStrengthUniform = glGetUniformLocation(m_program, std::string("directionallight.specularstrength").c_str());
	if (derectionalSpecularStrengthUniform != -1) {
		glUniform1fv(derectionalSpecularStrengthUniform, 1, &directional.specularStrength);
	}

	GLint derectionalLightColorUniform = glGetUniformLocation(m_program, std::string("directionallight.color").c_str());
	if (derectionalLightColorUniform != -1) {
		glUniform3fv(derectionalLightColorUniform, 1, glm::value_ptr(directional.color));
	}

	GLint derectionalLightDirUniform = glGetUniformLocation(m_program, std::string("directionallight.direction").c_str());
	if (derectionalLightDirUniform != -1) {
		glUniform3fv(derectionalLightDirUniform, 1, glm::value_ptr(directional.direction));
	}


	//set point lights
	for (int i = 0; i < pointLights.size(); i++) {
		std::string lightArrayItem = "pointlights[" + std::to_string(i) + "]";

		//set each property
		GLint ambientStrengthUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".ambientstrength").c_str());
		if (ambientStrengthUniform != -1) {
			glUniform1fv(ambientStrengthUniform, 1, &pointLights[i].ambientStrength);
		}
	
		GLint specularStrengthUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".specularstrength").c_str());
		if (specularStrengthUniform != -1) {
			glUniform1fv(specularStrengthUniform, 1, &pointLights[i].specularStrength);
		}

		GLint lightConstantUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".constant").c_str());
		if (lightConstantUniform != -1) {
			glUniform1fv(lightConstantUniform, 1, &pointLights[i].constant);
		}

		GLint lightLinearUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".linear").c_str());
		if (lightLinearUniform != -1) {
			glUniform1fv(lightLinearUniform, 1, &pointLights[i].linear);
		}

		GLint lightQuadraticUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".quadratic").c_str());
		if (lightQuadraticUniform != -1) {
			glUniform1fv(lightQuadraticUniform, 1, &pointLights[i].quadratic);
		}

		GLint lightColorUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".color").c_str());
		if (lightColorUniform != -1) {
			glUniform3fv(lightColorUniform, 1, glm::value_ptr(pointLights[i].color));
		}

		GLint lightPosUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".pos").c_str());
		if (lightPosUniform != -1) {
			glUniform3fv(lightPosUniform, 1, glm::value_ptr(pointLights[i].position));
		}
	}


	std::vector<SpotLight> spotLights = m_world->getLighting()->getSpotLights();
	//set spot lights
	for (int i = 0; i < spotLights.size(); i++) {
		std::string lightArrayItem = "spotlights[" + std::to_string(i) + "]";

		//set each property
		GLint lightColorUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".color").c_str());
		if (lightColorUniform != -1) {
			glUniform3fv(lightColorUniform, 1, glm::value_ptr(spotLights[i].color));
		}

		GLint lightPosUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".pos").c_str());
		if (lightPosUniform != -1) {
			glUniform3fv(lightPosUniform, 1, glm::value_ptr(spotLights[i].position));
		}

		GLint lightDirUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".direction").c_str());
		if (lightDirUniform != -1) {
			glUniform3fv(lightDirUniform, 1, glm::value_ptr(spotLights[i].direction));
		}

		GLint cutoffUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".cutoff").c_str());
		float calculatedCutoff = glm::cos(glm::radians(spotLights[i].cutoff));
		if (cutoffUniform != -1) {
			glUniform1fv(cutoffUniform, 1, &calculatedCutoff);
		}

		GLint ambientStrengthUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".ambientstrength").c_str());
		if (ambientStrengthUniform != -1) {
			glUniform1fv(ambientStrengthUniform, 1, &spotLights[i].ambientStrength);
		}

		GLint specularStrengthUniform = glGetUniformLocation(m_program, std::string(lightArrayItem + ".specularstrength").c_str());
		if (specularStrengthUniform != -1) {
			glUniform1fv(specularStrengthUniform, 1, &spotLights[i].specularStrength);
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
