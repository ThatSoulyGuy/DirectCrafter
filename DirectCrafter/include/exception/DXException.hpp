#ifndef DX_EXCEPTION
#define DX_EXCEPTION

#include <exception>
#include <string>
#include <sstream>

#define DX_EXEPTION_CATCH \
	\
	catch (const DXException& exception)\
	{ \
		MessageBox(nullptr, exception.what(), exception.GetType().c_str(), MB_OK | MB_ICONERROR); \
		return; \
	} \
	catch (const std::exception& exception) \
	{ \
		MessageBox(nullptr, exception.what(), "std::exception", MB_OK | MB_ICONERROR); \
		return; \
	} \
	catch (...) \
	{ \
		MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONERROR); \
		return; \
	}
		
class DXException : public std::exception
{

public:
	
	DXException(int line, const std::string& file) noexcept : line(line), file(file) {}

	const char* what() const noexcept override
	{
		std::ostringstream bufferStream;

		bufferStream << GetType() << std::endl << GetOriginString();
		
		whatBuffer = bufferStream.str();

		return whatBuffer.c_str();
	}

	virtual std::string GetType() const noexcept
	{
		return "DX Eception";
	}

	int GetLine() const noexcept
	{
		return line;
	}

	const std::string& GetFile() const noexcept
	{
		return file;
	}

	std::string GetOriginString() const noexcept
	{
		std::ostringstream originStream;

		originStream << "[File] " << file << std::endl << "[Line] " << line;

		return originStream.str();
	}

protected:

	mutable std::string whatBuffer;

private:

	int line = 0;
	std::string file = "";

};

#endif // !DX_EXCEPTION