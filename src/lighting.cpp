#include <lighting.h>

GW::RenderEngine::Lighting::Lighting()
{
}

GW::RenderEngine::Lighting::~Lighting()
{
}

void GW::RenderEngine::Lighting::setDirectionalLight(const DirectionalLight & light)
{
	m_directionalLight = light;
}

void GW::RenderEngine::Lighting::addPointLight(const PointLight & light)
{
	m_pointLights.push_back(light);
}

void GW::RenderEngine::Lighting::addSpotLight(const SpotLight & light)
{
	m_spotLights.push_back(light);
}

void GW::RenderEngine::Lighting::setPointLight(unsigned int index, const PointLight & light)
{
	if (index < m_pointLights.size()) {
		m_pointLights[index] = light;
	}
}

void GW::RenderEngine::Lighting::setSpotLight(unsigned int index, const SpotLight & light)
{
	if (index < m_spotLights.size()) {
		m_spotLights[index] = light;
	}
}

GW::RenderEngine::PointLight GW::RenderEngine::Lighting::getPointLight(unsigned int index)
{
	if (index < m_pointLights.size()) {
		return m_pointLights[index];
	}
	
	return GW::RenderEngine::PointLight();
}

std::vector<GW::RenderEngine::PointLight> GW::RenderEngine::Lighting::getPointLights()
{
	return m_pointLights;
}

std::vector<GW::RenderEngine::SpotLight> GW::RenderEngine::Lighting::getSpotLights()
{
	return m_spotLights;
}

GW::RenderEngine::DirectionalLight GW::RenderEngine::Lighting::getDirectionalLight()
{
	return m_directionalLight;
}
