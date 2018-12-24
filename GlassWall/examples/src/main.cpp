#include <world.h>
#include <entity.h>
#include <iostream>

class OneLiner : public GW::Entity {
	using GW::Entity::Entity;

	virtual void update() {
		std::cout << "Screw you! I'm leaving!" << std::endl;
		m_world->requestQuit();
	}
};

int main(int argc, char** argv) {
	GW::World world;

	OneLiner* dummy = new OneLiner("OneLiner");

	world.addEntity(dummy);

	world.start();

	delete dummy;

	return 0;
}