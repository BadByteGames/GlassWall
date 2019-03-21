#pragma once

//This file contains enumerators that are PUBLICLY used by the user
namespace GW {

	enum WINDOWFLAGS {FULLSCREEN = 0x1, BORDERLESS = 0x2, RESIZABLE = 0x4};

	enum AXISTYPE {SDLKEYBOARD = 0x1, 
		SDLMOUSEBUTTON,
		OTHER};
	
	//input codes for input value
	enum INPUTCODE {
		MOUSEYUP = 0x10,
		MOUSEYDOWN,
		MOUSEXRIGHT,
		MOUSEXLEFT,
		MWHEELUP = 0x20,
		MWHEELDOWN
	};

}