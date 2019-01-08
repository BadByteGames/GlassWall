#include <world.h>
#include <entity.h>
#include <iostream>
#include <model.h>
#include <component.h>
#include <shaderprogram.h>
#include <camera.h>
#include <textures.h>
#include <inputmanager.h>
#include <lighting.h>
#include <glm\gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\string_cast.hpp>

using GW::RenderEngine::ShaderProgram;
using GW::RenderEngine::Textures;

const float MOVE_SPEED = 2.0f;
const float SENSITIVITY = 120.0f;
const float PI = 3.14159265359f;

class OneLiner : public GW::Entity {
public:
	using GW::Entity::Entity;

	virtual void entityStart() {
		rootComponent = new GW::Component();

		//compile shaders on entity start
		m_shader.compileShadersFromFile("debug.vert", "debug.frag");

		//init the model
		m_model = new GW::RenderEngine::Model();;

		rootComponent->addChild(m_model);
		rootComponent->setAbsolutePosition(glm::vec3(0.0f, 0.0f, -4.0f));
		m_model->setRelativePosition(glm::vec3(0.0f, 0.0f, 0.0f));

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
		
		GW::InputManager* inputManager = m_world->getInputManager();
		GW::Axis rotateMonkey;
		rotateMonkey.axisInputs.emplace_back(GW::INPUTCODE::MWHEELUP, GW::AXISTYPE::OTHER, 1.0f);
		rotateMonkey.axisInputs.emplace_back(GW::INPUTCODE::MWHEELDOWN, GW::AXISTYPE::OTHER, -1.0f);
		inputManager->addAxis("rotatemonkey", rotateMonkey);

		GW::Axis lookHorizontal;
		lookHorizontal.limit = false;
		lookHorizontal.axisInputs.emplace_back(GW::INPUTCODE::MOUSEXRIGHT, GW::AXISTYPE::OTHER, -1.0f);
		lookHorizontal.axisInputs.emplace_back(GW::INPUTCODE::MOUSEXLEFT, GW::AXISTYPE::OTHER, 1.0f);
		inputManager->addAxis("lookhorizontal", lookHorizontal);


		GW::Axis lookVertical;
		lookVertical.limit = false;
		lookVertical.axisInputs.emplace_back(GW::INPUTCODE::MOUSEYUP, GW::AXISTYPE::OTHER, 1.0f);
		lookVertical.axisInputs.emplace_back(GW::INPUTCODE::MOUSEYDOWN, GW::AXISTYPE::OTHER, -1.0f);
		inputManager->addAxis("lookvertical", lookVertical);

		GW::Axis strafe;
		strafe.axisInputs.emplace_back(SDLK_d, GW::AXISTYPE::SDLKEYBOARD, 1.0f);
		strafe.axisInputs.emplace_back(SDLK_a, GW::AXISTYPE::SDLKEYBOARD, -1.0f);
		strafe.axisInputs.emplace_back(SDLK_RIGHT, GW::AXISTYPE::SDLKEYBOARD, 1.0f);
		strafe.axisInputs.emplace_back(SDLK_LEFT, GW::AXISTYPE::SDLKEYBOARD, -1.0f);
		inputManager->addAxis("moveright", strafe);

		GW::Axis moveforward;
		moveforward.axisInputs.emplace_back(SDLK_w, GW::AXISTYPE::SDLKEYBOARD, -1.0f);
		moveforward.axisInputs.emplace_back(SDLK_s, GW::AXISTYPE::SDLKEYBOARD, 1.0f);
		moveforward.axisInputs.emplace_back(SDLK_UP, GW::AXISTYPE::SDLKEYBOARD, -1.0f);
		moveforward.axisInputs.emplace_back(SDLK_DOWN, GW::AXISTYPE::SDLKEYBOARD, 1.0f);
		inputManager->addAxis("moveforward", moveforward);
	}

	virtual void update() {

		GW::RenderEngine::Camera* camera = m_world->getCamera();
		GW::InputManager* inputManager = m_world->getInputManager();
		GW::FpsCounter* fpsCounter = m_world->getFpsCounter();
		m_model->setRelativeOrientation(glm::vec3(0.0f, m_rotationY, 0.0f));

		glm::vec3 translation = glm::rotate(glm::mat4(1.0f), (angles.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(inputManager->getAxisValue("moveright") * MOVE_SPEED * fpsCounter->getDeltaTime(), 0.0f, inputManager->getAxisValue("moveforward") * MOVE_SPEED * fpsCounter->getDeltaTime(), 0.0f);
		glm::vec3 newPos = camera->getPosition() + translation;
		camera->setAbsolutePosition(newPos);

		
		
		//change camera angles based off mouse motion if mouse locked
		if (m_mouseLocked) {
			angles.y += inputManager->getAxisValue("lookhorizontal") * SENSITIVITY * fpsCounter->getDeltaTime();
			angles.x += inputManager->getAxisValue("lookvertical") * SENSITIVITY * fpsCounter->getDeltaTime();

			angles.x = glm::clamp(angles.x, -89.999f, 89.999f);

			camera->setRotation(angles);
		}

		//request quit on escape
		if (inputManager->isKeyDown(SDLK_ESCAPE)) {
			m_world->requestQuit();
		}

		m_rotationY += inputManager->getAxisValue("rotatemonkey") * 15.0f;

		if (inputManager->mousePressed(SDL_BUTTON_MIDDLE)) {
			if (m_mouseLocked) {
				m_mouseLocked = false;
				m_world->getInputManager()->setMouseTrapped(false);
			}
			else {
				m_mouseLocked = true;
				m_world->getInputManager()->setMouseTrapped(true);
			}
		}
	}

	virtual void cleanUp() {
		//delete shader
		m_shader.destroy();

		m_model->cleanUp();
		
		delete m_model;
		delete rootComponent;
	}

private:
	GW::RenderEngine::Model* m_model;

	float m_rotationY = 0.0f;

	bool m_mouseLocked = true;
	ShaderProgram m_shader;
	glm::vec3 angles;
};

int main(int argc, char** argv) {
	GW::World world;

	world.getLighting()->addLight(GW::RenderEngine::Light({ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 8.0f }, 0.5f, 0.1f));
	world.getLighting()->addLight(GW::RenderEngine::Light({ 1.0f, 0.0f, 0.0f }, { -3.0f, 0.0f, -7.0f }, 0.5f, 0.1f));
	world.getLighting()->addLight(GW::RenderEngine::Light({ 0.0f, 0.0f, 1.0f }, { 3.0f, 0.0f, -7.0f }, 0.5f, 0.1f));

	OneLiner* dummy = new OneLiner("OneLiner");

	world.addEntity(dummy);

	world.start();

	delete dummy;
	
	return 0;
}