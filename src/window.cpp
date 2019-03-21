#include <window.h>
#include <SDL.h>
#include <settings.h>
#include <GL/glew.h>
#include <iostream>

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
		windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (flags & WINDOWFLAGS::BORDERLESS) {
		windowFlags |= SDL_WINDOW_BORDERLESS;
	}
	if (flags & WINDOWFLAGS::RESIZABLE) {
		windowFlags |= SDL_WINDOW_RESIZABLE;
	}

	m_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);

	SDL_GL_CreateContext(m_window);
}

void GW::RenderEngine::Window::setFullscreen(bool value)
{
	if (m_window != nullptr) {
		SDL_SetWindowFullscreen(m_window, value ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}
}

void GW::RenderEngine::Window::setBorderless(bool value)
{
	if (m_window != nullptr) {
		SDL_SetWindowBordered(m_window, SDL_bool(!value));
	}
}

void GW::RenderEngine::Window::setResizable(bool value)
{
	if (m_window != nullptr) {
		SDL_SetWindowResizable(m_window, SDL_bool(value));
	}
}

void GW::RenderEngine::Window::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GW::RenderEngine::Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void GW::RenderEngine::Window::setTitle(std::string title)
{
	SDL_SetWindowTitle(m_window, title.c_str());
}

void GW::RenderEngine::Window::destroy()
{
	SDL_DestroyWindow(m_window);
}
