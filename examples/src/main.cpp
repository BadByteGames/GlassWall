#include <world.h>
#include <entity.h>
#include <iostream>
#include <model.h>
#include <shaderprogram.h>
#include <camera.h>
#include <textures.h>
#include <inputmanager.h>
#include <glm\gtc\matrix_transform.hpp>

using GW::RenderEngine::ShaderProgram;
using GW::RenderEngine::Textures;

const float MOVE_SPEED = 0.005f;
const float SENSITIVITY = 0.3f;
const float PI = 3.14159265359f;

class OneLiner : public GW::Entity {
public:
	using GW::Entity::Entity;

	virtual void entityStart() {
		//compile shaders on entity start
		m_shader.compileShadersFromFile("debug.vert", "debug.frag");

		//init the model
		m_model = new GW::RenderEngine::Model();

		//load model
		m_model->loadFromFile("test.dae");

		//capture mouse in window
		m_world->getInputManager()->setMouseTrapped(true);

		//set active shader of model
		m_model->useShader(m_shader);
		unsigned int texture = Textures::getTexture("MeshTexture.png");
		Textures::setTextureSlot(texture, 0);
		m_world->getCamera()->setOrthopgraphic(false);
		
		angles = glm::vec3(0.0f);
	}

	virtual void update() {
		if (!m_saidScrewYou) {
			std::cout << "Screw you! I'm leaving!" << std::endl;
			m_saidScrewYou = true;
		}

		GW::RenderEngine::Camera* camera = m_world->getCamera();
		GW::InputManager* inputManager = m_world->getInputManager();

		//quick wasd movement system
		if (inputManager->isKeyDown(SDLK_w)) {
			glm::vec3 translation = glm::rotate(glm::mat4(1.0f), (angles.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, -MOVE_SPEED, 0.0f);
			glm::vec3 newPos = camera->getPosition() + translation;
			camera->setAbsolutePosition(newPos);
		}
		if (inputManager->isKeyDown(SDLK_a)) {
			glm::vec3 translation = glm::rotate(glm::mat4(1.0f), (angles.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(-MOVE_SPEED, 0.0f, 0.0f, 0.0f);
			glm::vec3 newPos = camera->getPosition() + translation;
			camera->setAbsolutePosition(newPos);
		}
		if (inputManager->isKeyDown(SDLK_s)) {
			glm::vec3 translation = glm::rotate(glm::mat4(1.0f), (angles.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.0f, 0.0f, MOVE_SPEED, 0.0f);
			glm::vec3 newPos = camera->getPosition() + translation;
			camera->setAbsolutePosition(newPos);
		}
		if (inputManager->isKeyDown(SDLK_d)) {
			glm::vec3 translation = glm::rotate(glm::mat4(1.0f), (angles.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(MOVE_SPEED, 0.0f, 0.0f, 0.0f);
			glm::vec3 newPos = camera->getPosition() + translation;
			camera->setAbsolutePosition(newPos);
		}
		
		//change camera angles based off mouse motion
		angles.y += -(float)inputManager->getMouseData().xRel * SENSITIVITY;
		angles.x += (float)inputManager->getMouseData().yRel * SENSITIVITY;

		camera->setRotation(angles);

		//request quit on escape
		if (inputManager->isKeyDown(SDLK_ESCAPE)) {
			m_world->requestQuit();
		}
	}

	virtual void cleanUp() {
		//delete shader
		m_shader.destroy();

		m_model->cleanUp();
		
		delete m_model;
	}

private:
	float m_rotationY = 0.0f;

	bool m_saidScrewYou = false;
	ShaderProgram m_shader;
	glm::vec3 angles;
};

int main(int argc, char** argv) {
	GW::World world;

	OneLiner* dummy = new OneLiner("OneLiner");

	world.addEntity(dummy);

	world.start();

	delete dummy;

	return 0;
}