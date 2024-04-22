#include "OutStream.h"

#include <sstream>

namespace lip {
	OutStream cout;
}

void OutStream::setHwnd(HWND h) {
	hwnd = h;
}

HWND OutStream::getHwnd() {
	return hwnd;
}

std::wstring OutStream::stringToWstring(std::string text) {
	std::wstring wstr(text.begin(), text.end());
	return wstr;
}

std::string OutStream::intToString(int value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

OutStream& operator<<(OutStream& stream, std::string text) {
	std::wstring wstr = stream.stringToWstring(text);
	stream << wstr;
	return stream;
}

OutStream& operator<<(OutStream& stream, std::wstring text) {
	HWND temHwnd = lip::cout.getHwnd();
	//if (temHwnd == nullptr) return;
	int strLen = SendMessage(temHwnd, WM_GETTEXTLENGTH, 0, 0);
	SendMessage(temHwnd, EM_SETSEL, (WPARAM)strLen, (LPARAM)strLen);
	SendMessage(temHwnd, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)text.c_str());
	return stream;
}

OutStream& operator<<(OutStream& stream, int value) {
	std::wstring wstr = stream.stringToWstring(stream.intToString(value));
	stream << wstr;
	return stream;
}
