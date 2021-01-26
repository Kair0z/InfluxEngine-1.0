#include "pch.h"
#include "Timer.h"

Influx::Timer::Timer()
{
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mLastTime);

	mMaxDeltaTime = mFrequency.QuadPart / 10;
}

void Influx::Timer::Tick()
{
	LARGE_INTEGER currentTime;

	QueryPerformanceCounter(&currentTime);

	UINT64 deltaTime = currentTime.QuadPart - mLastTime.QuadPart;

	mLastTime = currentTime;
	mSecondCounter += deltaTime;

	if (deltaTime > mMaxDeltaTime)
	{
		deltaTime = mMaxDeltaTime;
	}

	deltaTime *= mTicksPerSecond;
	deltaTime /= mFrequency.QuadPart;

	UINT32 lastFrameCount = mFrameCount;
	mElapsedTicks = deltaTime;
	mTotalTicks += deltaTime;
	mLeftOverTicks = 0;
	mFrameCount++;

	if (mFrameCount != lastFrameCount)
	{
		mFrameOnThisSecond++;
	}

	if (mSecondCounter >= static_cast<UINT64>(mFrequency.QuadPart))
	{
		mFramesPerSecond = mFrameOnThisSecond;
		mFrameOnThisSecond = 0;
		mSecondCounter %= mFrequency.QuadPart;
	}
}

