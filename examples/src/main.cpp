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
#include <entities/staticblock.h>
#include <glm\gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\string_cast.hpp>

using GW::RenderEngine::ShaderProgram;
using GW::RenderEngine::Textures;

const float MOVE_SPEED = 2.0f;
const float SENSITIVITY = 1.0f;
const float PI = 3.14159265359f;

class Monkey : public GW::Entity {
public:
	Monkey() : GW::Entity() {

	}

	virtual void entityStart() {
		//compile shaders on entity start
		m_shader.compileShadersFromFile("debug.vert", "debug.frag");

		//init the model
		m_model = std::make_unique<GW::RenderEngine::Model>();

		rootComponent->addChild(m_model.get());
		m_model->setRelativePosition(glm::vec3(0.0f, 0.0f, 0.0f));

		//load model
		m_model->loadFromFile("test.dae");

		//set active shader of model
		m_model->useShader(m_shader);
		unsigned int texture = Textures::getTexture("MeshTexture.png");
		unsigned int specular = Textures::getTexture("MeshSpecular.png");
		m_model->setMaterial({ texture, specular });
	}

	virtual void update() {

		GW::InputManager* inputManager = m_world->getInputManager();
		GW::FpsCounter* fpsCounter = m_world->getFpsCounter();
		m_model->setOrientation(glm::vec3(0.0f, m_rotationY, 0.0f));

		m_rotationY += inputManager->getAxisValue("rotatemonkey") * 15.0f;
	}

	virtual void cleanUp() {
		//delete shader
		m_shader.destroy();

		m_model->cleanUp();
	}
	
	void setPos(glm::vec3 pos) {
		if (rootComponent != nullptr) {
			//set pos
			rootComponent->setAbsolutePosition(pos);
		}
	}

private:
	std::unique_ptr<GW::RenderEngine::Model> m_model;

	float m_rotationY = 0.0f;
	
	glm::vec3 m_startPos = glm::vec3(0.0f);
	bool m_mouseLocked = true;
	ShaderProgram m_shader;
};

class Player: public GW::Entity{
public:
	Player () : 
	GW::Entity()
	{
		m_camera = std::make_unique<GW::RenderEngine::Camera>();
		rootComponent->addChild(m_camera.get());
		m_camera->setRelativePosition(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	virtual void entityStart() {

		//compile shaders on entity start
		m_shader.compileShadersFromFile("debug.vert", "debug.frag");

		//capture mouse in window
		m_world->getInputManager()->setMouseTrapped(true);

		m_world->useCamera(m_camera.get());
		m_camera->setOrthopgraphic(false);

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

		GW::Axis fovChange;
		fovChange.axisInputs.emplace_back(SDLK_EQUALS, GW::AXISTYPE::SDLKEYBOARD, 1.0f);
		fovChange.axisInputs.emplace_back(SDLK_MINUS, GW::AXISTYPE::SDLKEYBOARD, -1.0f);
		inputManager->addAxis("fovchange", fovChange);
	}

	virtual void update() {

		GW::RenderEngine::Camera* camera = m_world->getCamera();
		GW::InputManager* inputManager = m_world->getInputManager();
		GW::FpsCounter* fpsCounter = m_world->getFpsCounter();

		glm::vec3 translation = glm::rotate(glm::mat4(1.0f), (angles.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::normalize(glm::vec4(inputManager->getAxisValue("moveright") * MOVE_SPEED * fpsCounter->getDeltaTime(), 0.0f, inputManager->getAxisValue("moveforward") * MOVE_SPEED * fpsCounter->getDeltaTime(), 1.0f));
		rootComponent->setAbsolutePosition(rootComponent->getAbsolutePosition() + translation);
		
		//change camera angles based off mouse motion if mouse locked
		if (m_mouseLocked) {
			angles.y += inputManager->getAxisValue("lookhorizontal") * SENSITIVITY;
			angles.x += inputManager->getAxisValue("lookvertical") * SENSITIVITY;

			angles.x = glm::clamp(angles.x, -89.999f, 89.999f);
			
			rootComponent->setOrientation(glm::vec3(0.0f, angles.y, 0.0f));
			m_camera->setOrientation(glm::vec3(angles.x, 0.0f, 0.0f));
		}

		GW::RenderEngine::SpotLight startLight = m_world->getLighting()->getSpotLights()[0];
		startLight.position = m_camera->getAbsolutePosition();
		startLight.direction =  glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f)) *  glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f))* glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		m_world->getLighting()->setSpotLight(0, startLight);

		//request quit on escape
		if (inputManager->isKeyDown(SDLK_ESCAPE)) {
			m_world->requestQuit();
		}

		//change fov for some trippy affects
		camera->setFOV(camera->getFov() + inputManager->getAxisValue("fovchange") * 0.5f);

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
	}

private:

	float m_rotationY = 0.0f;

	glm::vec3 m_startPos = glm::vec3(0.0f);
	bool m_mouseLocked = true;
	ShaderProgram m_shader;
	glm::vec3 angles;

	std::unique_ptr<GW::RenderEngine::Camera> m_camera;

	GW::StaticBlock* m_wall;
};

int main(int argc, char** argv) {
	GW::World world;

	world.getLighting()->addSpotLight(GW::RenderEngine::SpotLight({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, 0.5f, 0.1f, 9.5f, 25.5f, 1.0f, 0.14f, 0.07f));
	world.getLighting()->setDirectionalLight(GW::RenderEngine::DirectionalLight({ 1.0f, 1.0f, 1.0f }, {-0.2f, -1.0f, -0.2f}, 0.5f, 0.1f));

	world.addEntitySpawner("Monkey", []() {return std::move(std::make_unique<Monkey>()); });

	world.addEntity(std::make_unique<Player>());

	world.start("config.json");

	return 0;
}