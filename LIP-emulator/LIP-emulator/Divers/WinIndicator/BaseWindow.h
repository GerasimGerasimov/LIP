#pragma once

#include "BaseObject.h"

#include <vector>

class BaseWindow : public BaseObject
{
protected:
	void createIndicatorWindow();
public:
	BaseWindow(Parameter param);
	virtual void setData(std::vector<uint8_t>& vecData);
	virtual int getByteIndication();
};

