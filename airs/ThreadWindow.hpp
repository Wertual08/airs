#include "Window.hpp"
#include <thread>



namespace airs
{
	class ThreadWindow : public Window
	{
	private:
		bool Created;
		bool WorkerRunning;
		std::thread WindowWorker;

		void WorkerProcess(std::string title, int32_t w, int32_t h, int32_t x, int32_t y, Style s, StyleEx sx);

	public:
		ThreadWindow(const std::string& title, int32_t w, int32_t h, int32_t x = UseDefault, int32_t y = UseDefault, Style s = Style::Default, StyleEx sx = StyleEx::Default);
		~ThreadWindow();
	};
}