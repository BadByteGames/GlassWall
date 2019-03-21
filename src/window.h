#pragma once

#include <string>

struct SDL_Window;
namespace GW {
	namespace RenderEngine {
		class Window {
		public:
			Window();
			~Window();

			//creates the initial window
			void create(std::string name, int width, int height, unsigned int flags);

			//set window fullscreen state
			void setFullscreen(bool value);

			//set window bordered mode
			void setBorderless(bool value);

			//set window resizable
			void setResizable(bool value);
			
			/*call to clear a window at beginning of frame
			*rgba is on a scale of 0.0-1.0
			*/
			void clear(float r, float g, float b, float a = 1.0f);

			//call at end of frame to reset draw
			void swapBuffers();

			//sets window title
			void setTitle(std::string title);
			
			//call to clean up internal variables
			void destroy();
		private:
			SDL_Window* m_window = nullptr;
		};
	}
}