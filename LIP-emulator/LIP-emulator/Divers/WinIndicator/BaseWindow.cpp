#include "BaseWindow.h"

void BaseWindow::createIndicatorWindow() {
	hwnd = CreateWindow(L"static", L"", WS_VISIBLE /*| WS_CLIPCHILDREN*/ | WS_CHILD | WS_BORDER, rect.left, rect.top, width, height, Parrent,
		NULL, NULL, NULL);
}

BaseWindow::BaseWindow(Parameter param) : BaseObject(param){
	createIndicatorWindow();
}

void BaseWindow::setData(std::vector<uint8_t>& vecData) {}

int BaseWindow::getByteIndication() {
	return 0;
}
