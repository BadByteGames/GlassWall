#include <entities\staticblock.h>
#include <model.h>
#include <iostream>
#include <memory>
void GW::StaticBlock::entityStart()
{
	rootComponent = std::make_unique<GW::Component>();
	m_model = std::make_unique<GW::RenderEngine::Model>();

	rootComponent->addChild(m_model.get());
}

void GW::StaticBlock::cleanUp()
{
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
