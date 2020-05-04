#pragma once
#include <string>
#include <cstdint>
#include "Window.h"



namespace airs
{
	class MessageBox
	{
	public:
		enum class Result : int32_t
		{
			Ok =				1,
			Cancel =			2,
			Abort =				3,
			Retry =				4,
			Ignore =			5,
			Yes =				6,
			No =				7,
			TryAgain =			10,
			Continue =			11,
		};
		enum Button : uint32_t
		{
			Ok =				0x00000000L,
			OkCancel =			0x00000001L,
			AbortRetryIgnore =	0x00000002L,
			YesNoCancel =		0x00000003L,
			YesNo =				0x00000004L,
			RetryCancel =		0x00000005L,
			CancelTryContinue = 0x00000006L,
			Help =				0x00004000L,
		};
		enum Icon : uint32_t
		{
			Hand =				0x00000010L,
			Question =			0x00000020L,
			Exclamation =		0x00000030L,
			Asterisk =			0x00000040L,
			User =				0x00000080L,
			Warning =			Exclamation,
			Error =				Hand,
			Information =		Asterisk,
			Stop =				Hand,
		};
		enum Default : uint32_t
		{
			Button1 =			0x00000000L,
			Button2 =			0x00000100L,
			Button3 =			0x00000200L,
			Button4 =			0x00000300L,
		};
		enum Modal : uint32_t
		{
			App =				0x00000000L,
			System =			0x00001000L,
			Task =				0x00002000L,
		};
		enum Option : uint32_t
		{
			DesktopOnly =		0x00020000L,
			Riht =				0x00080000L,
			RTLReading =		0x00100000L,
			SetForeground =		0x00010000L,
			TopMost =			0x00040000L,
			ServiceNotification = 0x00200000L,
		};

		static Result Show(const std::string& message);
		static Result Show(const std::string& message, const std::string& caption);
		static Result Show(const std::string& message, uint32_t options);
		static Result Show(const std::string& message, const std::string& caption, uint32_t options);
		static Result Show(const Window& window, const std::string& message);
		static Result Show(const Window& window, const std::string& message, const std::string& caption);
		static Result Show(const Window& window, const std::string& message, uint32_t options);
		static Result Show(const Window& window, const std::string& message, const std::string& caption, uint32_t options);
	};
}
