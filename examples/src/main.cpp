#include <world.h>
#include <entity.h>
#include <iostream>
#include <model.h>
#include <shaderprogram.h>

using GW::RenderEngine::ShaderProgram;

class OneLiner : public GW::Entity {
public:
	using GW::Entity::Entity;

	virtual void entityStart() {
		//compile shaders on entity start
		m_shader.compileShadersFromFile("debug.vert", "debug.frag");

		//init the model
		m_model = new GW::RenderEngine::Model();

		//set up test data
		m_model->test();

		//set active shader of model
		m_model->useShader(m_shader);
	}

	virtual void update() {
		if (!m_saidScrewYou) {
			std::cout << "Screw you! I'm leaving!" << std::endl;
			m_saidScrewYou = true;
		}
		//m_world->requestQuit();
	}

	virtual void cleanUp() {
		//delete shader
		m_shader.destroy();

		m_model->cleanUp();
		
		delete m_model;
	}
private:
	bool m_saidScrewYou = false;
	ShaderProgram m_shader;
};

int main(int argc, char** argv) {
	GW::World world;

	OneLiner* dummy = new OneLiner("OneLiner");

	world.addEntity(dummy);

	world.start();

	delete dummy;

	return 0;
}