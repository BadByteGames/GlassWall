#pragma once

#include <string>

struct SDL_Window;
namespace GW {
	namespace RenderEngine {
		class Window {
		public:
			Window();
			~Window();

			void create(std::string name, int width, int height, unsigned int flags);

			void swapBuffers();

			void destroy();
		private:
			SDL_Window* m_window;
		};
	}
}