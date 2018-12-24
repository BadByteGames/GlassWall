#include "renderengine.h"

#include <SDL.h>
#include <iostream>

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

void GW::RenderEngine::quit()
{
	//quit out of SDL
	SDL_Quit();
	std::cout << "Quit SDL" << std::endl;
}