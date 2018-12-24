#pragma once

#include <string>

namespace GW {
	class World {
	public:
		World();
		~World();

		void test();
	private:
		std::string m_string = "hello there";
	};
}