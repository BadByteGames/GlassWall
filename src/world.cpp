#include <world.h>

#include <iostream>
#include <entity.h>
#include <renderengine.h>
#include <SDL.h>
#include <model.h>
#include <textures.h>
#include <component.h>
#include <window.h>
#include <json/json.hpp>
#include <util.h>
#include <entities/staticblock.h>


using nlohmann::json;
namespace GW {
	World::World() : m_requestQuit(false), m_worldStarted(false), m_windowFlags(0)
	{
		m_camera = std::make_unique<RenderEngine::Camera>();
		m_lighting = std::make_unique<RenderEngine::Lighting>();
		m_window = std::make_unique<RenderEngine::Window>();
		m_inputManager = std::make_unique<InputManager>();
		m_fpsCounter = std::make_unique<FpsCounter>();
	}

	World::~World()
	{
	}

	void World::start(std::string configName)
	{
		//load the config
		std::string configData = Util::getFileContents(configName);

		if (configData == "") {
			std::cout << "Failed to load config" << std::endl;
			return;
		}

		auto config = json::parse(configData);

		std::string windowName = "Glass Wall";
		if (config.find("window name") != config.end()) {
			windowName = config["window name"].get<std::string>();
		}
		

		//initialize rendering
		RenderEngine::init();

		//create a window
		m_window->create(windowName, 1280, 720, m_windowFlags);

		//init glew
		RenderEngine::initGL();

		m_camera->setDimensions(1280, 720);
		m_inputManager->setWindowDimensions(1280, 720);

		//trigger all the entity world start events
		for (int i = 0; i < m_entities.size(); i++) {
			m_entities[i]->onWorldStart();
		}

		m_worldStarted = true;

		//trigger any entity start events
		for (int i = 0; i < m_entities.size(); i++) {
			m_entities[i]->entityStart();
			initComponent(m_entities[i]->rootComponent.get());
		}

		if (config.find("start level") != config.end()) {
			loadFromFile(config["start level"].get<std::string>());
		}

		while (!m_requestQuit) {
			m_fpsCounter->startFrame();
			//clear the window to black
			m_window->clear(0.0f, 0.0f, 0.0f);

			//update input
			m_inputManager->update();

			//update the world until stopped
			update();

			//quit if necessary
			if (m_inputManager->quitRequested()) {
				m_requestQuit = true;
			}
						
			//draw entity models
			draw();

			//swap buffers
			m_window->swapBuffers();
			m_fpsCounter->endFrame();

			//set window title
			m_window->setTitle(windowName + " | "+std::to_string(m_fpsCounter->getFps())+" FPS");
		}

		//call entity cleanup events
		for (int i = 0; i < m_entities.size(); i++) {
			m_entities[i]->cleanUp();
		}

		//cleanup renderengine
		m_window->destroy();
		
		RenderEngine::Textures::clearCache();

		//don't delete entities managed by user
	}

	void World::addEntity(std::unique_ptr<Entity> entity)
	{
		entity->m_world = this;
		GW::Entity* tempRef = entity.get();
		m_entities.push_back(std::move(entity));

		//init entity if world already started
		if (m_worldStarted) {
			tempRef->entityStart();
			initComponent(tempRef->rootComponent.get());
		}
	}

	void World::setWindowFullscreen(bool value)
	{
		if (m_worldStarted) {
			m_window->setFullscreen(value);
		}
		else {
			m_windowFlags ^= (-(value ? 1 : 0)) & WINDOWFLAGS::FULLSCREEN;
		}
	}

	void World::setWindowBorderless(bool value)
	{
		m_window->setBorderless(value);
	}

	void World::setWindowResizable(bool value)
	{
		m_window->setResizable(value);
	}

	void World::setWindowFlags(unsigned int value)
	{
		m_windowFlags = value;
	}

	RenderEngine::Camera * World::getCamera()
	{
		return m_camera.get();
	}

	InputManager * World::getInputManager()
	{
		return m_inputManager.get();
	}

	RenderEngine::Lighting * World::getLighting()
	{
		return m_lighting.get();
	}

	FpsCounter * World::getFpsCounter()
	{
		return m_fpsCounter.get();
	}

	bool World::getWorldStarted()
	{
		return m_worldStarted;
	}

	void World::loadFromFile(const std::string & fileName)
	{
		std::string levelFile = Util::getFileContents(fileName);
		if (levelFile == "") {
			std::cout << "Failed to open: "<<fileName << std::endl;
		}
		auto levelJson = json::parse(levelFile);

		std::unordered_map < std::string, RenderEngine::Material > materials;
		//generate materials
		if (levelJson.find("materials") != levelJson.end()) {
			for (auto it : levelJson["materials"].items()) {
				RenderEngine::Material tempMaterial(0, 0);

				if (it.value().find("texture") != it.value().end()) {
					tempMaterial.diffuseID = RenderEngine::Textures::getTexture(it.value()["texture"].get<std::string>());
				}

				if (it.value().find("specular") != it.value().end()) {
					tempMaterial.specularID = RenderEngine::Textures::getTexture(it.value()["specular"].get<std::string>());

				}

				materials.insert(std::make_pair(it.key(), tempMaterial));
			}
		}

		std::unordered_map < std::string, GW::RenderEngine::ShaderProgram> programs;
		//load shaders
		if (levelJson.find("shaders") != levelJson.end()) {
			for (auto it : levelJson["shaders"].items()) {
				GW::RenderEngine::ShaderProgram tempProgram;

				std::string vertex = "";
				if (it.value().find("vertex") != it.value().end()) {
					vertex = it.value()["vertex"].get<std::string>();
				}

				std::string fragment = "";
				if (it.value().find("fragment") != it.value().end()) {
					fragment = it.value()["fragment"].get<std::string>();
				}

				tempProgram.compileShadersFromFile(vertex, fragment);
				programs.insert(std::make_pair(it.key(), tempProgram));
			}
		}
		
		//generate static blocks
		if (levelJson.find("blocks") != levelJson.end()) {
			for (auto it : levelJson["blocks"].items()) {
				std::unique_ptr<GW::StaticBlock> blockUnique = std::make_unique<GW::StaticBlock>();
				GW::StaticBlock* block = blockUnique.get();
				this->addEntity(std::move(blockUnique));
				//load dimensions
				if (it.value().find("dims") != it.value().end()) {
					auto dims = it.value()["dims"];
					glm::vec3 dimValues(0.0f);

					if (dims.find("x") != dims.end()) {
						dimValues.x = dims["x"].get<float>();
					}

					if (dims.find("y") != dims.end()) {
						dimValues.y = dims["y"].get<float>();
					}

					if (dims.find("z") != dims.end()) {
						dimValues.z = dims["z"].get<float>();
					}

					block->setDimensions(dimValues);
				}

				//load position
				if (it.value().find("pos") != it.value().end()) {
					auto pos = it.value()["pos"];
					glm::vec3 posValues(0.0f);

					if (pos.find("x") != pos.end()) {
						posValues.x = pos["x"].get<float>();
					}

					if (pos.find("y") != pos.end()) {
						posValues.y = pos["y"].get<float>();
					}

					if (pos.find("z") != pos.end()) {
						posValues.z = pos["z"].get<float>();
					}

					block->setPosition(posValues);
				}

				//load orientation
				if (it.value().find("orientation") != it.value().end()) {
					auto orientation = it.value()["orientation"];
					glm::vec3 orientationValues(0.0f);

					if (orientation.find("x") != orientation.end()) {
						orientationValues.x = orientation["x"].get<float>();
					}

					if (orientation.find("y") != orientation.end()) {
						orientationValues.y = orientation["y"].get<float>();
					}

					if (orientation.find("z") != orientation.end()) {
						orientationValues.z = orientation["z"].get<float>();
					}

					block->setOrientation(orientationValues);
				}

				//load the shader
				if (it.value().find("shader") != it.value().end()) {
					block->setShader(programs.find(it.value()["shader"])->second);
				}
				
				//load the material
				if (it.value().find("material") != it.value().end()) {
					block->useMaterial(materials.find(it.value()["material"])->second);
				}
			}
		}

	}

	void World::update()
	{
		//call all entity update functions
		for (int i = 0; i < m_entities.size(); i++) {
			m_entities[i]->update();
			updateComponent(m_entities[i]->rootComponent.get());
		}
	}
	void World::draw()
	{
		for (int i = 0; i < m_entities.size(); i++) {
			drawComponent(m_entities[i]->rootComponent.get());
		}
	}

	void World::initComponent(Component * component)
	{
		if (component != nullptr) {
			component->m_world = this;
			for (auto it : component->getChildren()) {
				initComponent(it);
			}
		}
	}

	void World::updateComponent(Component * component)
	{
		if (component != nullptr) {
			component->update();
			for (auto it : component->getChildren()) {
				updateComponent(it);
			}
		}
	}

	void World::drawComponent(Component * component)
	{	
		if (component != nullptr) {
			component->draw();
			for (auto it : component->getChildren()) {
				drawComponent(it);
			}
		}
	}
}