#pragma once
#include <cstdint>



namespace airs
{
	class Timer
	{
	private:
		bool Stopped;
		uint64_t InvFreqMilli;
		uint64_t StartCount;
		mutable uint64_t CurrentCount;

	public:
		Timer();
		~Timer();

		void Start();
		void Reset();
		void Stop();
		double Elapsed() const;
	};


	class FrameTimer
	{
	private:
		double Sensitivity;
		Timer Clocker;
		double TimeMin;
		double TimeMax;

		double LastBuffer;
		double LastCount;
		double TimeBuffer;
		double FramesCount;

	public:
		FrameTimer(double sensitivity = 1.0);
		~FrameTimer();

		void SetSensitivity(double sen);

		void StartFrame();
		void StopFrame();
		void Reset();
		double GetAvg() const;
		double GetMin() const;
		double GetMax() const;
	};
}