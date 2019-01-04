#include <inputmanager.h>
#include <SDL.h>
#include <iostream>

GW::InputManager::InputManager()
{
	m_quitRequested = false;
}

GW::InputManager::~InputManager()
{
}

void GW::InputManager::update()
{
	//handle all events
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			//user requested quit
			m_quitRequested = true;
			break;
		case SDL_KEYDOWN:
		{
			//update our key to down if found
			auto it = m_keyValues.find(evnt.key.keysym.sym);
			if (it != m_keyValues.end()) {
				//key found, update
				it->second = true;
			}
			else {
				//create entry for key
				m_keyValues.insert(std::make_pair(evnt.key.keysym.sym, true));
			}
			break;
		}
		case SDL_KEYUP:
		{
			//update our key to up if found
			auto it = m_keyValues.find(evnt.key.keysym.sym);
			if (it != m_keyValues.end()) {
				//key found, update
				it->second = false;
			}
			else {
				//create entry for key
				m_keyValues.insert(std::make_pair(evnt.key.keysym.sym, false));
			}
			break;
		}
		default:
			//unrecognized event, dev is lazy
			std::cout << "Unrecognized event type: " << evnt.type << "!\nPlease contact a dev with this information." << std::endl;
		}
	}
}

bool GW::InputManager::isKeyDown(int keycode)
{
	//check if key stored in data and then return value if found or false if not
	auto it = m_keyValues.find(keycode);
	if (it != m_keyValues.end()) {
		//key found, return value
		return it->second;
	}
	return false;
}

bool GW::InputManager::quitRequested()
{
	return m_quitRequested;
}
