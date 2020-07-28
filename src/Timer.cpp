#include "airs/Timer.hpp"
#include "AIRSWin.hpp"
#include <algorithm>
#include <chrono>



namespace airs
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;

	Timer::Timer()
	{
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&InvFreqMilli));
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
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&StartCount));
	}
	void Timer::Reset()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&StartCount));
		Stopped = false;
	}
	void Timer::Stop()
	{
		if (Stopped) return;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&CurrentCount));
		Stopped = true;
	}
	double Timer::Elapsed() const
	{
		if (!Stopped) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&CurrentCount));
		return (double)(CurrentCount - StartCount) / (double)InvFreqMilli;
	}


	FrameTimer::FrameTimer(double sensitivity) :
		Sensitivity(sensitivity),
		TimeMin(DBL_MAX),
		TimeMax(DBL_MAX),
		LastBuffer(0.0),
		LastCount(0.0),
		TimeBuffer(0.0),
		FramesCount(0.0)
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
		TimeMax = std::max(TimeMax, FrameTime);

		TimeBuffer += FrameTime;
		FramesCount += 1.0;

		if (TimeBuffer > Sensitivity)
		{
			LastBuffer = TimeBuffer;
			LastCount = FramesCount;
			TimeBuffer = 0;
			FramesCount = 0;
		}
	}
	void FrameTimer::Reset()
	{
		TimeMin = DBL_MAX;
		TimeMax = DBL_MAX;
		LastBuffer = 0.0;
		LastCount = 0.0;
		TimeBuffer = 0.0;
		FramesCount = 0.0;
	}
	double FrameTimer::GetAvg() const
	{
		return (LastBuffer + TimeBuffer) / (LastCount + FramesCount);
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