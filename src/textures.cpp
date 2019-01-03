#include <textures.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

std::unordered_map<std::string, unsigned int> GW::RenderEngine::Textures::m_textureMap;

unsigned int GW::RenderEngine::Textures::getTexture(const std::string & fileName, bool useNearest)
{
	//check if image already loaded into memory
	auto it = m_textureMap.find(fileName);
	if (it != m_textureMap.end()) {
		std::cout << "Cached texture: " << fileName << std::endl;

		//texture already loaded, return id
		return it->second;
	}
	else {
		//load image
		stbi_set_flip_vertically_on_load(true);

		//generate a texture
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		//load texture to generate settings
		glBindTexture(GL_TEXTURE_2D, textureId);

		//set up texture to repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//use nearest or linear interpolation
		if (useNearest) {
			//sets texture to have pixelated values when resized
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			//sets texture to "blur" when modified
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		//load the texture we are going to use from the file
		int width, height, nrChannels;
		unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 4);

		//load texture into OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//free data used by stbi
		stbi_image_free(data);

		//unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		//cache the texture id
		m_textureMap.insert(std::make_pair(fileName, textureId));

		std::cout << "Loaded texture: " << fileName << std::endl;

		//return the id
		return textureId;
	}

	//something has gone wrong
	return 0;
}

void GW::RenderEngine::Textures::setTextureSlot(const unsigned int & textureId, const unsigned int & slot)
{
	//set slot
	switch (slot) {
	case 0:
		glActiveTexture(GL_TEXTURE0);
		break;
	case 1:
		glActiveTexture(GL_TEXTURE1);
		break;
	case 2:
		glActiveTexture(GL_TEXTURE2);
		break;
	case 3:
		glActiveTexture(GL_TEXTURE3);
		break;
	case 4:
		glActiveTexture(GL_TEXTURE4);
		break;
	case 5:
		glActiveTexture(GL_TEXTURE5);
		break;
	case 6:
		glActiveTexture(GL_TEXTURE6);
		break;
	case 7:
		glActiveTexture(GL_TEXTURE7);
		break;
	case 8:
		glActiveTexture(GL_TEXTURE8);
		break;
	case 9:
		glActiveTexture(GL_TEXTURE9);
		break;
	case 10:
		glActiveTexture(GL_TEXTURE10);
		break;
	case 11:
		glActiveTexture(GL_TEXTURE11);
		break;
	case 12:
		glActiveTexture(GL_TEXTURE12);
		break;
	case 13:
		glActiveTexture(GL_TEXTURE13);
		break;
	case 14:
		glActiveTexture(GL_TEXTURE14);
		break;
	case 15:
		glActiveTexture(GL_TEXTURE15);
		break;
	}

	//set texture
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void GW::RenderEngine::Textures::clearCache()
{
	for (auto it : m_textureMap) {
		glDeleteTextures(1, &it.second);
	}

	m_textureMap.clear();
}
