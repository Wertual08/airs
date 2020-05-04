#include "airs/Timer.h"
#include "AIRSWin.h"
#include <algorithm>
#include <chrono>



namespace airs
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;

	Timer::Timer()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*)& InvFreqMilli);
		CurrentCount = 0;
		StartCount = 0;
		Stopped = true;
	}
	Timer::~Timer()
	{
	}

	void Timer::Start()
	{
		if (!Stopped) return;
		Stopped = false;
		QueryPerformanceCounter((LARGE_INTEGER*)& StartCount);
	}
	void Timer::Reset()
	{
		QueryPerformanceCounter((LARGE_INTEGER*)& StartCount);
		Stopped = false;
	}
	void Timer::Stop()
	{
		if (Stopped) return;
		QueryPerformanceCounter((LARGE_INTEGER*)& CurrentCount);
		Stopped = true;
	}
	double Timer::Elapsed() const
	{
		if (!Stopped) QueryPerformanceCounter((LARGE_INTEGER*)& CurrentCount);
		return (double)(CurrentCount - StartCount) / (double)InvFreqMilli;
	}


	FrameTimer::FrameTimer(double sensitivity) :
		Sensitivity(sensitivity),
		TimeMin(DBL_MAX),
		TimeAvg(0.0),
		TimeMax(DBL_MAX)
	{
	}
	FrameTimer::~FrameTimer()
	{
	}

	void FrameTimer::SetSensitivity(double sen)
	{
		Sensitivity = sen;
	}

	void FrameTimer::StartFrame()
	{
		Clocker.Start(); 
	}
	void FrameTimer::StopFrame()
	{
		Clocker.Stop();
		const double FrameTime = Clocker.Elapsed();
		TimeMin = std::min(TimeMin, FrameTime);
		TimeAvg += (FrameTime - TimeAvg) * std::min(1.0, FrameTime) * Sensitivity;
		TimeMax = std::max(TimeMax, FrameTime);
	}
	void FrameTimer::Reset()
	{
		TimeMin = DBL_MAX;
		TimeAvg = 0.0;
		TimeMax = DBL_MAX;
	}
	double FrameTimer::GetAvg() const
	{
		return TimeAvg;
	}
	double FrameTimer::GetMin() const
	{
		return TimeMin;
	}
	double FrameTimer::GetMax() const
	{
		return TimeMax;
	}
}