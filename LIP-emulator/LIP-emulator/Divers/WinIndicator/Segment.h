#pragma once

#include "BaseWindow.h"

class Segment : public BaseWindow
{
private:
	HBRUSH brushOn;
	HBRUSH brushOff;
	//HDC hdc;
public:
	Segment(Parameter param);
	void turnOn(HDC Hdc);
	void turnOff(HDC Hdc);
	//void init() override;
};

