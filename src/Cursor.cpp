#include "airs/Cursor.hpp"
#include "airs/encoding.hpp"
#include "AIRSWin.hpp"



namespace airs
{
	Cursor::Cursor(int, void* c)
	{
		Handle = c;
	}
	Cursor::Cursor()
	{
		Handle = 0;
	}
	Cursor::Cursor(const std::string& file)
	{
		Handle = LoadCursorFromFileW(to_wide(file).c_str());
	}
	Cursor::Cursor(int x, int y, int w, int h, const void* andm, const void* xorm)
	{
		Handle = CreateCursor(GetModuleHandleW(nullptr), x, y, w, h, andm, xorm);
	}
	Cursor::~Cursor()
	{
		DestroyCursor(static_cast<HCURSOR>(Handle));
	}
	bool Cursor::operator==(const Cursor& c)
	{
		return Handle == c.Handle;
	}

	const Cursor Cursor::AppStarting(	0, LoadCursorW(0, IDC_APPSTARTING));
	const Cursor Cursor::Arrow(			0, LoadCursorW(0, IDC_ARROW));
	const Cursor Cursor::Cross(			0, LoadCursorW(0, IDC_CROSS));
	const Cursor Cursor::Hand(			0, LoadCursorW(0, IDC_HAND));
	const Cursor Cursor::Help(			0, LoadCursorW(0, IDC_HELP));
	const Cursor Cursor::IBeam(			0, LoadCursorW(0, IDC_IBEAM));
	const Cursor Cursor::Icon(			0, LoadCursorW(0, IDC_ICON));
	const Cursor Cursor::No(			0, LoadCursorW(0, IDC_NO));
	const Cursor Cursor::Size(			0, LoadCursorW(0, IDC_SIZE));
	const Cursor Cursor::SizeAll(		0, LoadCursorW(0, IDC_SIZEALL));
	const Cursor Cursor::SizeNESW(		0, LoadCursorW(0, IDC_SIZENESW));
	const Cursor Cursor::SizeNS(		0, LoadCursorW(0, IDC_SIZENS));
	const Cursor Cursor::SizeNWSE(		0, LoadCursorW(0, IDC_SIZENWSE));
	const Cursor Cursor::SizeWE(		0, LoadCursorW(0, IDC_SIZEWE));
	const Cursor Cursor::UpArrow(		0, LoadCursorW(0, IDC_UPARROW));
	const Cursor Cursor::Wait(			0, LoadCursorW(0, IDC_WAIT));
}