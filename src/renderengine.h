#pragma once

namespace GW {
	namespace RenderEngine{
		//init SDL and any other subsystems
		void init();

		//init OpenGL and glew MUST BE CALLED AFTER WINDOW CREATION
		void initGL();

		//clean up internal variables
		void quit();
	}
}