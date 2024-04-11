#pragma once

#include "BaseWindow.h"

//конкретный сегмент
class Segment : public BaseWindow
{
private:
	HBRUSH brushOn;
	HBRUSH brushOff;
	HDC hdc;
public:
	Segment(Parameter param);
	void turnOn(HDC Hdc);
	void turnOff();

};

