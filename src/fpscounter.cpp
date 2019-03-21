#include <fpscounter.h>
#include <SDL.h>
#include <iostream>

GW::FpsCounter::FpsCounter()
{
	for (int i = 0; i < FRAME_SAMPLES; i++) {
		m_frameArray[i] = 0;
	}
	m_currentIndex = 0;
	m_lastFrame = 0;
	m_deltaTime = 0;
}

GW::FpsCounter::~FpsCounter()
{
}

void GW::FpsCounter::startFrame()
{
	m_lastFrame = SDL_GetTicks();
}

void GW::FpsCounter::endFrame()
{
	unsigned int current = SDL_GetTicks();
	m_frameArray[m_currentIndex] = current - m_lastFrame;
	m_deltaTime = (1.0f / 1000.0f) * (float)m_frameArray[m_currentIndex];

	m_lastFrame = current;
	m_currentIndex++;
	m_currentIndex = m_currentIndex % FRAME_SAMPLES;
}

float GW::FpsCounter::getFps()
{
	float totalTime = 0.0f;
	for (int i = 0; i < FRAME_SAMPLES; i++) {
		totalTime += (float)m_frameArray[i];
	}
	totalTime = totalTime / (float)FRAME_SAMPLES;
	return 1000.0f / totalTime;
}

float GW::FpsCounter::getDeltaTime()
{
	return m_deltaTime;
}
