#pragma once

#include "IndicatorContainer.h"
#include "WinLIP_5Nx.h"

class WinLIPModule : public IndicatorContainer
{
private:
	int count;
public:
	WinLIPModule(Parameter param, int CountIndicator, RECT newBorder);
	void createSegment();
};

//width 430
//height 130 + 110 + 10