#ifndef WIN_EXCEPTION_HPP
#define WIN_EXCEPTION_HPP

#include <Windows.h>
#include "exception/DXException.hpp"

#define DXWIN_EXCEPTION(result) WinException(__LINE__, __FILE__, result)
#define DXWIN_LAST_EXCEPTION() WinException(__LINE__, __FILE__, GetLastError())

class WinException : public DXException
{

public:
	
	WinException(int line, const char* file, HRESULT result) noexcept : DXException(line, file), result(result) {}

	const char* what() const noexcept override
	{
		std::ostringstream whatStream;

		whatStream << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorString() << std::endl
			<< GetOriginString();

		whatBuffer = whatStream.str();

		return whatBuffer.c_str();
	}

	HRESULT GetErrorCode() const noexcept
	{
		return result;
	}

	std::string GetErrorString() const noexcept
	{
		return TranslateErrorCode(result);
	}

	static std::string TranslateErrorCode(HRESULT result) noexcept
	{
		char* messageBuffer = nullptr;

		DWORD messageLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

		if (messageLength == 0)
			return "Unidentified error code";

		std::string errorString = messageBuffer;
		LocalFree(messageBuffer);

		return errorString;
	}

private:

	HRESULT result;

};

#endif // !WIN_EXCEPTION_HPP