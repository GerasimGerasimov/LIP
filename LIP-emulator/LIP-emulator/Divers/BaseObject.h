#pragma once

#include "framework.h"

class BaseObject
{
protected:
	HWND hwnd = NULL;
	HWND Parrent;
	RECT rect;
	int width;
	int height;
public:
	struct Parameter {
		HWND parrent;
		RECT rect;
	};
	BaseObject(Parameter param);
	virtual ~BaseObject();
};

