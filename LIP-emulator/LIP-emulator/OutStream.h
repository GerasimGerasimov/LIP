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
};

	OutStream& operator<<(OutStream& stream, std::string text);
	OutStream& operator<<(OutStream& stream, std::wstring text);

namespace lip {
	extern OutStream cout;
}