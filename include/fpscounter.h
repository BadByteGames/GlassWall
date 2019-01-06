#pragma once

namespace GW {
	//const value describing frame samples
	const int FRAME_SAMPLES = 10;

	class FpsCounter {
	public:
		FpsCounter();
		~FpsCounter();

		void startFrame();

		void endFrame();

		float getFps();
	private:
		unsigned int m_lastFrame;

		int m_currentIndex;

		unsigned int m_frameArray[FRAME_SAMPLES];
	};
}