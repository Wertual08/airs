#pragma once
#include <string>



namespace airs
{
	class Cursor
	{
		friend class Window;
	private:
		explicit Cursor(int, void* c);

	protected:
		void* Handle;

	public:
		Cursor();
		Cursor(const std::string& file);
		Cursor(int x, int y, int w, int h, const void* andm, const void* xorm);
		Cursor(const Cursor& c) = delete;
		~Cursor();
		Cursor& operator=(const Cursor& c) = delete;
		bool operator==(const Cursor& c);

		static const Cursor AppStarting;
		static const Cursor Arrow;
		static const Cursor Cross;
		static const Cursor Hand;
		static const Cursor Help;
		static const Cursor IBeam;
		static const Cursor Icon;
		static const Cursor No;
		static const Cursor Size;
		static const Cursor SizeAll;
		static const Cursor SizeNESW;
		static const Cursor SizeNS;
		static const Cursor SizeNWSE;
		static const Cursor SizeWE;
		static const Cursor UpArrow;
		static const Cursor Wait;
	};
}

