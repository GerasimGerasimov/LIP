#include "BaseWindow.h"

void BaseWindow::init() {
	hwnd = CreateWindow(L"static", L"", WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD | WS_BORDER, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Parrent,
		NULL, NULL, NULL);
}

BaseWindow::BaseWindow(Parameter param) {
	Parrent = param.parrent;
	rect.left = param.rect.left;
	rect.top = param.rect.top;
	rect.right = param.rect.right;
	rect.bottom = param.rect.bottom;

	//init();
}

BaseWindow::~BaseWindow() {}
