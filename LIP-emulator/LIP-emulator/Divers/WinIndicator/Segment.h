#pragma once

#include "BaseWindow.h"

//конкретный сегмент
class Segment : public BaseWindow
{
private:
	HBRUSH brushOn;
	HBRUSH brushOff;
	HDC hdc;
	void setColor(HBRUSH brush);
public:
	Segment(Parameter param);
	void setData(std::vector<uint8_t>& vecData) override;

};

