#pragma once

//expose keycodes to user
#include <SDL_keycode.h>
#include <SDL_mouse.h>

#include <unordered_map>
#include <string>
#include <settings.h>

namespace GW {
	//structure for mouse motion data
	struct MouseMotion {
		int xPos = 0;
		int yPos = 0;
		int xRel = 0;
		int yRel = 0;
	};

	//structure use forinput in an axis
	struct AxisInput {
		AxisInput(){}
		AxisInput(int code, GW::AXISTYPE type, float scale):code(code), type(type), scale(scale){}

		int code; //what to check for input, use sdl codes for keyboard and mouse button values but GW::INPUTCODE for other things
		GW::AXISTYPE type; //stores the type of input the axis will be using use SDLKEYBOARD for key board and SDLMOUSEBUTTON for mouse buttons but OTHER for every other type
		float scale; //what to multiply value by
	};

	struct Axis {
		std::vector<AxisInput> axisInputs; //inputs to use
		bool limit = true; //wheter it should clamp or not
		float limitUpper = 1.0f; //what to clamp to if limit is true
		float limitLower = -1.0f; //what to clamp to if limit is true
	};

	class InputManager {
	public:
		InputManager();
		~InputManager();

		//called every frame to refresh data
		void update();

		//get is key down
		bool isKeyDown(int keycode);

		//gets if mouse button down
		bool isMouseButtonDown(int buttoncode);

		//get if a key was pressed
		bool keyPressed(int keycode);

		//get if a mouse button was pressed
		bool mousePressed(int buttoncode);

		//gets whether QUIT has been called
		bool quitRequested();

		//get mouse data
		GW::MouseMotion getMouseData();

		//traps the mouse in the window
		void setMouseTrapped(bool value);

		//sets window dimensions for internal calculations
		void setWindowDimensions(int width, int height);

		//get mouse wheel movement
		// +1 is up
		// 0 is no movement
		// -1 is down
		int getMouseWheelMovement();

		//add an -1 to +1 axis to check for
		void addAxis(std::string name, Axis axis);

		//gets value of an axis
		//returns 0 if not found
		float getAxisValue(std::string name);
	private:
		float getValueDirection(int code, GW::AXISTYPE type);

		//stores key values
		std::unordered_map<int, bool> m_keyValues;

		//stores mouse values
		std::unordered_map<int, bool> m_mouseButtons;

		//values from last frame
		std::unordered_map<int, bool> m_keyValuesLast;
		std::unordered_map<int, bool> m_mouseButtonsLast;

		//has user requested quit
		bool m_quitRequested;

		//mouse motion data
		MouseMotion m_mouseMotion;

		//window dimensions
		int m_width = 0;
		int m_height = 0;

		//stores mwheel movement
		int m_wheelMovement = 0;

		//stores all axes
		std::unordered_map<std::string, Axis> m_axes;
	};
}