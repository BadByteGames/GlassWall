#include <world.h>
#include <entity.h>
#include <iostream>

class OneLiner : public GW::Entity {
public:
	using GW::Entity::Entity;

	virtual void update() {
		if (!m_saidScrewYou) {
			std::cout << "Screw you! I'm leaving!" << std::endl;
			m_saidScrewYou = true;
		}
		//m_world->requestQuit();
	}
private:
	bool m_saidScrewYou = false;
};

int main(int argc, char** argv) {
	GW::World world;

	OneLiner* dummy = new OneLiner("OneLiner");

	world.addEntity(dummy);

	world.start();

	delete dummy;

	return 0;
}