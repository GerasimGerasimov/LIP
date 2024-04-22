#pragma once

#include "framework.h"

#include <string>

class OutStream
{
private:
	HWND hwnd = NULL;

public:
	void setHwnd(HWND h);
	HWND getHwnd();
	std::wstring stringToWstring(std::string text);
	std::string intToString(int value);
};

	OutStream& operator<<(OutStream& stream, std::string text);
	OutStream& operator<<(OutStream& stream, std::wstring text);
	OutStream& operator<<(OutStream& stream, int value);

namespace lip {
	extern OutStream cout;
}