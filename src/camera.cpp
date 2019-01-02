#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
const float PI = 3.14159265359f;

GW::RenderEngine::Camera::Camera()
{
	m_position = glm::vec3(0.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_target = glm::vec3(0.0f, 0.0f, -1.0f);
	m_rotations = glm::vec3(0.0f, 0.0f, 0.0f);

	m_orthographic = false;
	m_lookat = false;

	m_width = 1920;
	m_height = 1080;
	m_fov = 60.0f;
}

GW::RenderEngine::Camera::~Camera()
{
}

void GW::RenderEngine::Camera::setAbsolutePosition(const glm::vec3 & position)
{
	m_position = position;
}

void GW::RenderEngine::Camera::setRotation(const glm::vec3& rotations)
{
	m_rotations = glm::vec3((rotations.x * PI) / 180.0f, (rotations.y * PI) / 180.0f, (rotations.z * PI) / 180.0f);
}

void GW::RenderEngine::Camera::setRotation(float x, float y, float z)
{
	m_rotations = glm::vec3((x * PI) / 180.0f, (y * PI) / 180.0f, (z * PI) / 180.0f);
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

void GW::RenderEngine::Camera::setTargetLock(const bool & value)
{
	m_lookat = value;
}

void GW::RenderEngine::Camera::update()
{
	//only update if not in m_lookat mode
	if (!m_lookat) {
		m_target.x = cos(m_rotations.x) * sin(m_rotations.y);
		m_target.y = cos(m_rotations.x) * cos(m_rotations.y);
		m_target.z = sin(m_rotations.x);
		
		//add in location
		m_target += m_position;

		m_up.x = cos(m_rotations.x + (90.0f * PI) / 180.0f) * sin(m_rotations.z);
		m_up.y = cos(m_rotations.x + (90.0f * PI) / 180.0f) * cos(m_rotations.z);
		m_up.z = sin(m_rotations.x + (90.0f * PI) / 180.0f);

		m_up += m_position;
	}
}

void GW::RenderEngine::Camera::setDimensions(const int & width, const int & height)
{
	m_width = width;
	m_height = height;
}

glm::mat4 GW::RenderEngine::Camera::getViewMatrix()
{
	glm::mat4 viewMatrix = glm::lookAt(m_position, m_target, m_up);
	
	return viewMatrix;
}

glm::mat4 GW::RenderEngine::Camera::getProjectionMatrix()
{
	glm::mat4 projection = glm::mat4(1.0f);

	float aspectRatio = (float)m_width / (float)m_height;

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
	m_fov = fov;
}
