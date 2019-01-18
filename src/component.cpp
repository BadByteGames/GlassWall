#include <component.h>

#include <glm/gtc/matrix_transform.hpp>
#include <world.h>
#include <iostream>

const float PI = 3.14159265359f;

GW::Component::Component()
{
	m_parent = nullptr;
	m_absoluteMode = false;
	m_world = nullptr;
	m_type = "component";
	m_absolutePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_absoluteOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_relativeOrientation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_relativePosition = glm::vec3(0.0f, 0.0f, 0.0f);
}

GW::Component::~Component()
{
}

void GW::Component::update()
{
}

void GW::Component::draw()
{

}

void GW::Component::addChild(Component * component)
{
	component->m_parent = this;
	m_children.push_back(component);
}

void GW::Component::setAbsolutePosition(glm::vec3 position)
{
	m_absoluteMode = true;
	m_absolutePosition = position;
}

void GW::Component::setRelativePosition(glm::vec3 position)
{
	m_absoluteMode = false;
	m_relativePosition = position;
}

void GW::Component::setAbsoluteOrientation(glm::vec3 angles)
{
	m_absoluteOrientation = glm::vec3((angles.x * PI) / 180.0f, (angles.y * PI) / 180.0f, (angles.z * PI) / 180.0f);
}

void GW::Component::setRelativeOrientation(glm::vec3 angles)
{
	m_relativeOrientation = glm::vec3((angles.x * PI) / 180.0f, (angles.y * PI) / 180.0f, (angles.z * PI) / 180.0f);
}

glm::mat4 GW::Component::getTransform()
{
	if (m_absoluteMode) {
		//just return the transform
		glm::mat4 transformation = glm::mat4(1.0f);
		transformation = glm::translate(transformation, m_absolutePosition);
		
		transformation = glm::rotate(transformation, m_absoluteOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transformation = glm::rotate(transformation, m_absoluteOrientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transformation = glm::rotate(transformation, m_absoluteOrientation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		return transformation;
	}
	//return transform of parent modified
	glm::mat4 transformation = glm::mat4(1.0f);
	if (m_parent != nullptr) {
		transformation = m_parent->getTransform();
	}

	if(getAbsolutePosition() != glm::vec3(0.0f))
		transformation = glm::translate(transformation, m_relativePosition);
	transformation = glm::rotate(transformation, m_relativeOrientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transformation = glm::rotate(transformation, m_relativeOrientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transformation = glm::rotate(transformation, m_relativeOrientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	return transformation;
}

std::vector<GW::Component*> GW::Component::getChildren()
{
	return m_children;
}

GW::Component * GW::Component::getParent()
{
	return m_parent;
}

glm::vec3 GW::Component::getAbsolutePosition()
{
	if (m_absoluteMode) {
		return m_absolutePosition;
	}
	else {
		glm::vec4 absoluteTranslation = glm::rotate(glm::mat4(1.0f), (m_relativeOrientation.x * PI) / 180.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(m_relativePosition, 1.0f);
		absoluteTranslation = glm::rotate(glm::mat4(1.0f), (m_relativeOrientation.y * PI) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * absoluteTranslation;
		absoluteTranslation = glm::rotate(glm::mat4(1.0f), (m_relativeOrientation.z * PI) / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f)) * absoluteTranslation;
		if (m_parent != nullptr) {
			return m_parent->getAbsolutePosition() + glm::vec3(absoluteTranslation);
		}
		else {
			return glm::vec3(absoluteTranslation);
		}
	}
}

glm::vec3 GW::Component::getRelativePosition()
{
	if (m_absoluteMode) {
		if (m_parent != nullptr) {
			return m_absolutePosition - m_parent->getAbsolutePosition();
		}
		else {
			return m_absolutePosition;
		}
	}
	else {
		return m_relativePosition;
	}
}
