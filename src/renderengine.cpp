#include "renderengine.h"

#include <SDL.h>
#include <iostream>
#include <GL/glew.h>

void GW::RenderEngine::init()
{
	//just initialize SDL for now
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "Error: " << SDL_GetError() << std::endl;
	}
	else {
		std::cout << "SDL Initialized" << std::endl;
	}
}

void GW::RenderEngine::initGL()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	else {
		std::cout << "glew Initialized" << std::endl;
	}

	//init depth for 3D
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//init alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GW::RenderEngine::quit()
{
	//quit out of SDL
	SDL_Quit();
	std::cout << "Quit SDL" << std::endl;
}