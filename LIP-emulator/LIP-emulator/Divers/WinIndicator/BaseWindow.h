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
	virtual void init();
public:
	struct Parameter{
		HWND parrent;
		RECT rect;
	};
	
	BaseWindow(Parameter param);
	virtual ~BaseWindow();
};

