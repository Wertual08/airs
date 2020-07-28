#include "airs\ThreadWindow.hpp"
#include <Windows.h>



void airs::ThreadWindow::WorkerProcess(std::string title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx)
{
	Init(title, w, h, x, y, s, sx);
	Created = true;
	MSG msg;
	while (WorkerRunning && GetMessageW(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

airs::ThreadWindow::ThreadWindow(const std::string& title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx)
{
	Created = false;
	WorkerRunning = true;
	WindowWorker = std::thread(&ThreadWindow::WorkerProcess, this, title, w, h, x, y, s, sx);
	while (!Created) std::this_thread::sleep_for(std::chrono::duration<double>(0.01));
}
airs::ThreadWindow::~ThreadWindow()
{
	Created = false;
	WorkerRunning = false;
	PostMessageW(nullptr, 0, 0, 0);
	if (WindowWorker.joinable()) WindowWorker.join();
}
