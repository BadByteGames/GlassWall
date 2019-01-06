#include <lighting.h>

GW::RenderEngine::Lighting::Lighting()
{
	m_ambientColor = glm::vec3(1.0f);
	m_ambientStrength = 0.1f;
}

GW::RenderEngine::Lighting::~Lighting()
{
}

void GW::RenderEngine::Lighting::setAmbient(const glm::vec3 & ambientColor, const float & ambientStrength)
{
	setAmbientStrength(ambientStrength);
	setAmbientColor(ambientColor);
}

void GW::RenderEngine::Lighting::setAmbientStrength(const float & ambientStrength)
{
	m_ambientStrength = ambientStrength;
}

void GW::RenderEngine::Lighting::setAmbientColor(const glm::vec3 & ambientColor)
{
	m_ambientColor = ambientColor;
}

float GW::RenderEngine::Lighting::getAmbientStrength()
{
	return m_ambientStrength;
}

glm::vec3 GW::RenderEngine::Lighting::getAmbientColor()
{
	return m_ambientColor;
}

void GW::RenderEngine::Lighting::addLight(const Light & light)
{
	m_lights.push_back(light);
}

std::vector<GW::RenderEngine::Light> GW::RenderEngine::Lighting::getLights()
{
	return m_lights;
}
