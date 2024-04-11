#pragma once

#include "framework.h"

class BaseWindow
{
protected:
	HWND hwnd = NULL;
	HWND Parrent;
	RECT rect;
	int width;
	int height;
public:
	struct Parameter{
		HWND parrent;
		RECT rect;
	};
	virtual void init();
	BaseWindow(Parameter param);
	virtual ~BaseWindow();
};

