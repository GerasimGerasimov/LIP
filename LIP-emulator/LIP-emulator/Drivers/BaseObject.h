#pragma once

#include "framework.h"

#include <vector>

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
	virtual void setData(std::vector<uint8_t>& vecData);
	virtual int getByteIndication();
};

