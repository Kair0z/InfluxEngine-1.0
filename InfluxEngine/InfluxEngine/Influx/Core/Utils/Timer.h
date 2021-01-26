#pragma once

namespace Influx
{
	class Timer
	{
	public:
		Timer();
		IFX_DelCpyMove(Timer);

		void Tick();

		const UINT64 GetElapsedTicks() const { return mElapsedTicks; }
		const UINT32 GetFramesPerSecond() const { return mFramesPerSecond; }

	private:
		LARGE_INTEGER mFrequency;
		LARGE_INTEGER mLastTime;
		UINT64 mMaxDeltaTime;

		static const UINT64 mTicksPerSecond = 10000000;

		UINT32 mFrameCount = 0;
		UINT32 mFramesPerSecond = 0;
		UINT32 mFrameOnThisSecond = 0;
		UINT64 mSecondCounter = 0;

		UINT64 mElapsedTicks;
		UINT64 mTotalTicks;
		UINT64 mLeftOverTicks;

	};

}


