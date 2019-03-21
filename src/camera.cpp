#include "camera.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
const float PI = 3.14159265359f;

GW::RenderEngine::Camera::Camera():
	Component()
{
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_target = glm::vec3(0.0f, 0.0f, -1.0f);
	m_type = "camera";

	m_orthographic = false;
	m_lookat = false;

	m_width = 1920;
	m_height = 1080;
	m_fov = glm::radians(60.0f);
}

GW::RenderEngine::Camera::~Camera()
{
}

void GW::RenderEngine::Camera::setTarget(const glm::vec3 & target)
{
	m_target = target;
}

void GW::RenderEngine::Camera::setUpVector(const glm::vec3 & up)
{
	m_up = up;
}

void GW::RenderEngine::Camera::setOrthopgraphic(const bool & value)
{
	m_orthographic = value;
}

void GW::RenderEngine::Camera::useTarget(const bool & value)
{
	m_lookat = value;
}

void GW::RenderEngine::Camera::setDimensions(const int & width, const int & height)
{
	m_width = width;
	m_height = height;
}

glm::mat4 GW::RenderEngine::Camera::getViewMatrix()
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);

	if (m_lookat) {
		viewMatrix = glm::lookAt(getAbsolutePosition(), m_target, m_up);
	}
	else {
		viewMatrix = glm::inverse(getTransform());
	}

	return viewMatrix;
}

glm::mat4 GW::RenderEngine::Camera::getProjectionMatrix()
{
	glm::mat4 projection = glm::mat4(1.0f);

	float aspectRatio = ((float)m_width) / ((float)m_height);

	if (m_orthographic) {
		projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 1000.0f);
	}
	else {
		projection = glm::perspective(m_fov, aspectRatio, 0.1f, 1000.0f);
	}

	return projection;
}

void GW::RenderEngine::Camera::setFOV(const float & fov)
{
	m_fov = glm::radians(fov);
}

float GW::RenderEngine::Camera::getFov()
{
	return glm::degrees(m_fov);
}
