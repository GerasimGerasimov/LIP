#pragma once

#include "IndicatorContainer.h"
#include "Win7Segment.h"

//плата LIP.5Nx
class WinLIP_5Nx : public IndicatorContainer
{
private:

	const int count = 5;
public:
	WinLIP_5Nx(Parameter param);
	void createIndicator();
	//void turnOff();

};

