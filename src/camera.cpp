#include "camera.h"

GW::RenderEngine::Camera::Camera()
{
	m_position = glm::vec3(0.0f);
}

GW::RenderEngine::Camera::~Camera()
{
}

void GW::RenderEngine::Camera::setAbsolutePosition(const glm::vec3 & position)
{
}
