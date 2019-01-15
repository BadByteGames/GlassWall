#pragma once

#include <string>
#include <unordered_map>

namespace GW {
	namespace RenderEngine {
		struct Material {
			Material(){ }
			Material(unsigned int diffuseID, unsigned int specularID): diffuseID(diffuseID), specularID(specularID){}
			unsigned int diffuseID;
			unsigned int specularID;
		};

		class Textures {
		public:
			Textures();
			~Textures();

			//get the id of a texture
			//useNearest represents wheter interpolation should be more pixelated
			static unsigned int getTexture(const std::string& fileName, bool useNearest = false);

			//set a texture slot to a certain texture 
			//texture slot is a number between 0-16 but model class usually has slot 0 reserved
			static void setTextureSlot(const unsigned int& textureId, const unsigned int& slot);

			//clears texture cache in order to reload any textures 
			//DO NOT CALL unless if you know what you are doing
			static void clearCache();
		private:
			//stores all texture ids
			static std::unordered_map<std::string, unsigned int> m_textureMap;
		};
	};
};