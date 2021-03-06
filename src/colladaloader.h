#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <vertex.h>

namespace GW {
	namespace RenderEngine {
		struct VertexProperty {
			std::string source;
			int offset;
		};

		//this class is meant to load a collada file's data a present it in an array
		class ColladaLoader {
		public:
			ColladaLoader();
			~ColladaLoader();

			//load the image from a file
			void loadModel(std::string fileName);

			std::vector<GW::RenderEngine::Vertex> getVertices();
		private:
			//function to run if not cached
			void loadFromFile(std::string fileName);

			//list of source values
			std::unordered_map<std::string, std::vector<float>> m_sources;

			//list of vertex values
			std::unordered_map<std::string, std::string> m_vertexAliases;

			//indicies of properties
			std::vector<int> m_indicies;

			std::vector<GW::RenderEngine::Vertex> m_vertices;

			//store all loaded meshes for caching
			static std::unordered_map < std::string, std::vector<GW::RenderEngine::Vertex>> m_meshes;
		};
	}
};