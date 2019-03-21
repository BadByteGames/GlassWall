#pragma once

namespace GW {
	//const value describing frame samples
	const int FRAME_SAMPLES = 30;

	class FpsCounter {
	public:
		FpsCounter();
		~FpsCounter();

		void startFrame();

		void endFrame();

		float getFps();

		float getDeltaTime();
	private:
		unsigned int m_lastFrame;

		float m_deltaTime;

		int m_currentIndex;

		unsigned int m_frameArray[FRAME_SAMPLES];
	};
}