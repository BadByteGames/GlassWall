#include <entities\staticblock.h>
#include <model.h>
#include <iostream>

void GW::StaticBlock::entityStart()
{
	rootComponent = new GW::Component();
	m_model = new GW::RenderEngine::Model();

	rootComponent->addChild(m_model);
}

void GW::StaticBlock::cleanUp()
{
	delete m_model;
	delete rootComponent;
}

void GW::StaticBlock::setPosition(glm::vec3 pos)
{
	m_model->setAbsolutePosition(pos);
}

void GW::StaticBlock::setOrientation(glm::vec3 angles)
{
	m_model->setOrientation(angles);
}

void GW::StaticBlock::setDimensions(glm::vec3 dims)
{
	m_model->setBox(dims);
}

void GW::StaticBlock::setShader(const RenderEngine::ShaderProgram & program)
{
	m_model->useShader(program);
}

void GW::StaticBlock::useMaterial(const GW::RenderEngine::Material & material)
{
	m_model->setMaterial(material);
}
