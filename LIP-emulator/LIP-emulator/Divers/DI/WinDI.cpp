#include "WinDI.h"

#include <string>
#include <sstream>

void WinDI::createButtonDI() {
	
	for (int i = 0; i < countDI; ++i) {
		std::string nameDI = "DI_";
		std::stringstream ss;
		ss << i;
		nameDI += ss.str();
		HWND hwndDI = CreateWindowA("button", nameDI.c_str(), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP | WS_BORDER, rectButton.left, rectButton.top, widthButton, heightButton, hwnd,
			NULL, NULL, NULL);
		ButtonDI.push_back(hwndDI);
		OffsetRect(&rectButton, 0, heightButton);
	}
}

void WinDI::create() {
	hwnd = CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, rect.left, rect.top, width, height, Parrent,
		NULL, NULL, NULL);
}

WinDI::WinDI(Parameter param) : BaseObject(param){
	widthButton = 65;
	heightButton = 20;
	width = widthButton;
	height = heightButton * countDI;

	create();
	rectButton = {0, 0, widthButton, heightButton };
	createButtonDI();
}
