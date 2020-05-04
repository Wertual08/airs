#include "airs/PreciseSleeper.h"
#include <Windows.h>



namespace airs
{
    PreciseSleeper::PreciseSleeper()
    {
        TimerHandle = CreateWaitableTimerW(nullptr, true, nullptr);
    }
    PreciseSleeper::~PreciseSleeper()
    {
        CloseHandle(TimerHandle);
    }

    void PreciseSleeper::Sleep(double sec)
    {
        LARGE_INTEGER li;
        li.QuadPart = -(LONGLONG)(sec * 10000000.0);
        if (SetWaitableTimer(TimerHandle, &li, 0, NULL, NULL, FALSE))
            WaitForSingleObject(TimerHandle, INFINITE);
    }
}