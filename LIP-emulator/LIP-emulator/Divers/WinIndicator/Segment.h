#pragma once

#include "BaseWindow.h"

//конкретный сегмент
class Segment : public BaseObject
{
private:
	HBRUSH brushOn;
	HBRUSH brushOff;
	HDC hdc;
	void setColor(HBRUSH brush);
	void createSegment();
public:
	Segment(Parameter param);
	void setData(std::vector<uint8_t>& vecData) override;

};

