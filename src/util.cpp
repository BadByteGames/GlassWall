#include <util.h>

#include <fstream>
std::string GW::Util::getFileContents(const std::string & fileName)
{
	std::string str = "";
	std::ifstream t;
	t.open(fileName, std::ios::in | std::ios::binary);

	if (t.is_open()) {
		t.seekg(0, std::ios::end);
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
	}

	return str;
}
