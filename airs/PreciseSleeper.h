#pragma once



namespace airs
{
	class PreciseSleeper
	{
	private:
		void* TimerHandle;

	public:
		PreciseSleeper();
		~PreciseSleeper();

		void Sleep(double sec);
	};
}
