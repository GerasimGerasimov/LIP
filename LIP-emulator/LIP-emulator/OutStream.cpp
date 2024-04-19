#include "OutStream.h"

namespace lip {
	OutStream cout;
}

void OutStream::setHwnd(HWND h) {
	hwnd = h;
}

HWND OutStream::getHwnd() {
	return hwnd;
}

OutStream& operator<<(OutStream& stream, std::string text) {
	std::wstring wstr(text.begin(), text.end());
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
