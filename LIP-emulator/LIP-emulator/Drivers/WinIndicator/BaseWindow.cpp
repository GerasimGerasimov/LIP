#include "BaseWindow.h"

void BaseWindow::createIndicatorWindow() {
	hwnd = CreateWindow(L"static", L"", WS_VISIBLE /*| WS_CLIPCHILDREN*/ | WS_CHILD | WS_BORDER, rect.left, rect.top, width, height, Parrent,
		NULL, NULL, NULL);
}

BaseWindow::BaseWindow(Parameter param) : BaseObject(param){
	createIndicatorWindow();
}