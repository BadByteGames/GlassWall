#include <window.h>
#include <SDL.h>
#include <settings.h>

GW::RenderEngine::Window::Window()
{
}

GW::RenderEngine::Window::~Window()
{
}

void GW::RenderEngine::Window::create(std::string name, int width, int height, unsigned int flags)
{
	//create the window with specified flags
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	if (flags & WINDOWFLAGS::FULLSCREEN) {
		windowFlags &= SDL_WINDOW_FULLSCREEN;
	}
	if (flags & WINDOWFLAGS::BORDERLESS) {
		windowFlags &= SDL_WINDOW_BORDERLESS;
	}
	if (flags & WINDOWFLAGS::RESIZABLE) {
		windowFlags &= SDL_WINDOW_RESIZABLE;
	}

	m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);

	SDL_GL_CreateContext(m_window);
}

void GW::RenderEngine::Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void GW::RenderEngine::Window::destroy()
{
	SDL_DestroyWindow(m_window);
}
