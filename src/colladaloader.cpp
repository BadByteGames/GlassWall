#include <colladaloader.h>

#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const float PI = 3.14159265359f;

using tinyxml2::XMLNode;
using tinyxml2::XMLElement;
using tinyxml2::XMLAttribute;
using tinyxml2::XMLText;

GW::RenderEngine::ColladaLoader::ColladaLoader()
{
}

GW::RenderEngine::ColladaLoader::~ColladaLoader()
{
}

void GW::RenderEngine::ColladaLoader::loadFromFile(std::string fileName)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(fileName.c_str()) == 0) {

		//get model data
		XMLElement* root = doc.FirstChildElement("COLLADA");
		XMLElement* geometries = NULL;
		XMLElement* asset = NULL;
		if (root != NULL) {
			asset = root->FirstChildElement("asset");
			geometries = root->FirstChildElement("library_geometries");
		}
		else {
			std::cout << "COLLADA is NULL" << std::endl;
		}

		std::string upAxis = "Y_UP";
		if (asset != NULL) {
			XMLElement* upAxisElement = asset->FirstChildElement("up_axis");
			if (upAxisElement != NULL) {
				upAxis = upAxisElement->GetText();
			}
		}

		XMLElement* firstGeometry = NULL;
		if (geometries != NULL) {
			firstGeometry = geometries->FirstChildElement();
		}
		else {
			std::cout << "library_geometries is NULL" << std::endl;
		}

		XMLElement* mesh = NULL;
		if (firstGeometry != NULL) {
			mesh = firstGeometry->FirstChildElement("mesh");
		}

		//loop through sources
		if (mesh != NULL) {
			tinyxml2::XMLElement* meshProperty = mesh->FirstChildElement();
			while (meshProperty != NULL) {
				if (std::string(meshProperty->Name()) == "source") {
					std::vector<float> temp;
					//look through float array
					XMLElement* floatArray = meshProperty->FirstChildElement("float_array");
					std::vector<std::string> stringArray;

					std::string floatText = floatArray->GetText();
					std::istringstream iss(floatText);
					for (std::string s; iss >> s; )
						stringArray.push_back(s);

					for (auto it : stringArray) {
						temp.push_back(std::stof(it));
					}

					m_sources.insert(std::make_pair("#"+std::string(meshProperty->Attribute("id")), temp));
				}
				else if (std::string(meshProperty->Name()) == "vertices") {
					XMLElement* posSource = meshProperty->FirstChildElement("input");
					m_vertexAliases.insert(std::make_pair("#" + std::string(meshProperty->Attribute("id")), std::string(posSource->Attribute("source"))));
				}
				else if (std::string(meshProperty->Name()) == "triangles") {
					std::unordered_map<std::string, VertexProperty> properties;
					XMLElement* vertexProperty = meshProperty->FirstChildElement("input");
					while (vertexProperty != NULL) {
						VertexProperty sourceEval;
						sourceEval.offset = std::stoi(vertexProperty->Attribute("offset"));

						auto sourcesIt = m_sources.find(vertexProperty->Attribute("source"));
						if (sourcesIt != m_sources.end()) {
							sourceEval.source = vertexProperty->Attribute("source");
						}
						else {
							sourceEval.source = m_vertexAliases.find(vertexProperty->Attribute("source"))->second;
						}

						properties.insert(std::make_pair(vertexProperty->Attribute("semantic"), sourceEval));
						vertexProperty = vertexProperty->NextSiblingElement("input");
					}

					XMLElement* p = meshProperty->FirstChildElement("p");
					
					std::vector<std::string> stringArray;

					std::string indexText = p->GetText();
					std::istringstream iss(indexText);
					for (std::string s; iss >> s; )
						stringArray.push_back(s);

					for (auto it : stringArray) {
						m_indicies.push_back(std::stoi(it));
					}

					m_vertices.resize(std::stoi(meshProperty->Attribute("count")) * 3, Vertex());

					for (auto it : properties) {
						unsigned int vertexIndex = 0;
						unsigned int index = it.second.offset;

						//get property source
						auto source = m_sources.find(it.second.source)->second;
						if (it.first == "VERTEX") {
							//loop through attribute
							for (; index < m_indicies.size(); index += (unsigned int)properties.size()) {
								unsigned int sourceIndex = (unsigned int)m_indicies[index] * 3;
								glm::vec4 sourceVert = glm::vec4(source[sourceIndex], source[sourceIndex + 1], source[sourceIndex + 2], 0.0f);
								//rotate vertex if up axis is different
								if (upAxis == "Y_UP") {
									//do nothing
								}
								else if (upAxis == "Z_UP") {
									//rotate by 90 degrees
									sourceVert = glm::rotate(glm::mat4(1.0f), (-90.0f  * PI) / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * sourceVert;
								}
								else {
									//warn of unknown axis
									std::cout << "Unknown up axis: " << upAxis << std::endl;
								}

								m_vertices[vertexIndex].position = Position(sourceVert.x, sourceVert.y, sourceVert.z);
								vertexIndex++;
							}
						}
						else if (it.first == "TEXCOORD") {
							//loop through attribute
							for (; index < m_indicies.size(); index += (unsigned int)properties.size()) {
								unsigned int sourceIndex = (unsigned int)m_indicies[index] * 2;
								m_vertices[vertexIndex].uv = Uv(source[sourceIndex], source[sourceIndex + 1]);
								vertexIndex++;
							}
						}
						else if (it.first == "NORMAL") {
							//loop through attribute
							for (; index < m_indicies.size(); index += (unsigned int)properties.size()) {
								unsigned int sourceIndex = (unsigned int)m_indicies[index] * 3;
								glm::vec4 sourceVert = glm::vec4(source[sourceIndex], source[sourceIndex + 1], source[sourceIndex + 2], 0.0f);
								//rotate vertex if up axis is different
								if (upAxis == "Y_UP") {
									//do nothing
								}
								else if (upAxis == "Z_UP") {
									//rotate by 90 degrees
									sourceVert = glm::rotate(glm::mat4(1.0f), (-90.0f  * PI) / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * sourceVert;
								}
								else {
									//warn of unknown axis
									std::cout << "Unknown up axis: " << upAxis << std::endl;
								}

								m_vertices[vertexIndex].normal = Normal(sourceVert.x, sourceVert.y, sourceVert.z);
								vertexIndex++;
							}
						}
						
					}
				}

				meshProperty = meshProperty->NextSiblingElement();
			}
		}

		
	}
	else {
		std::cout << "Couldn't load file: " << fileName << std::endl;
	}
}

std::vector<GW::RenderEngine::Vertex> GW::RenderEngine::ColladaLoader::getVertices()
{
	return m_vertices;
}
