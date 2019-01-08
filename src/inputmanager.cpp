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
	//reset mouse motion relative
	m_mouseMotion.xRel = 0;
	m_mouseMotion.yRel = 0;

	//reset mouse wheel movement
	m_wheelMovement = 0;

	//handle all events
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			//user requested quit
			m_quitRequested = true;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			//update our key to up if found
			auto it = m_keyValues.find(evnt.key.keysym.sym);
			if (it != m_keyValues.end()) {
				//key found, update
				it->second = evnt.key.state;
			}
			else {
				//create entry for key
				m_keyValues.insert(std::make_pair(evnt.key.keysym.sym, evnt.key.state));
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		{
			//update our key to up if found
			auto it = m_mouseButtons.find(evnt.button.button);
			if (it != m_mouseButtons.end()) {
				//key found, update
				it->second = evnt.button.state;
			}
			else {
				//create entry for key
				m_mouseButtons.insert(std::make_pair(evnt.button.button, evnt.button.state));
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			//handle mouse wheel movement
			m_wheelMovement = evnt.wheel.y;
			break;
		}
		case SDL_MOUSEMOTION:
		{
			//update mouse motion values
			//y rel and ypos are inverted
			m_mouseMotion.xPos = evnt.motion.x;
			m_mouseMotion.yPos = m_height - evnt.motion.y;
			m_mouseMotion.xRel = evnt.motion.xrel;
			m_mouseMotion.yRel = -evnt.motion.yrel;
			break;
		}
		default:
			break;
			//unrecognized event, event should be handled
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

bool GW::InputManager::isMouseButtonDown(int keycode)
{
	//check if button stored in data and then return value if found or false if not
	auto it = m_mouseButtons.find(keycode);
	if (it != m_mouseButtons.end()) {
		//key found, return value
		return it->second;
	}
	return false;
}

bool GW::InputManager::quitRequested()
{
	return m_quitRequested;
}

GW::MouseMotion GW::InputManager::getMouseData()
{
	return m_mouseMotion;
}

void GW::InputManager::setMouseTrapped(bool value)
{
	SDL_SetRelativeMouseMode(SDL_bool(value));
}

void GW::InputManager::setWindowDimensions(int width, int height)
{
	m_width = width;
	m_height = height;
}

int GW::InputManager::getMouseWheelMovement()
{
	return m_wheelMovement;
}
