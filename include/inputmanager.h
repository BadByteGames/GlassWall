#pragma once

//expose keycodes to user
#include <SDL_keycode.h>

#include <unordered_map>
#include <string>

namespace GW {
	class InputManager {
	public:
		InputManager();
		~InputManager();

		//called every frame to refresh data
		void update();

		//get is key down
		bool isKeyDown(int keycode);

		//gets whether QUIT has been called
		bool quitRequested();
	private:
		//stores key values
		std::unordered_map<int, bool> m_keyValues;

		//has user requested quit
		bool m_quitRequested;
	};
}