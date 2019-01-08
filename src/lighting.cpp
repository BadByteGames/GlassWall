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

void GW::RenderEngine::Lighting::addLight(const Light & light)
{
	m_lights.push_back(light);
}

std::vector<GW::RenderEngine::Light> GW::RenderEngine::Lighting::getLights()
{
	return m_lights;
}

GW::RenderEngine::DirectionalLight GW::RenderEngine::Lighting::getDirectionalLight()
{
	return m_directionalLight;
}
