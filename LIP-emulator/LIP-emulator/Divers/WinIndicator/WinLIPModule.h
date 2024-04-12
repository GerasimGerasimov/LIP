#pragma once

#include "IndicatorContainer.h"
#include "WinLIP_5Nx.h"

class WinLIPModule : public IndicatorContainer
{
private:
	int count;
	static const int widthChildIndicator = 410;
	static const int heightChildIndicator = 110;
public:
	WinLIPModule(Parameter param, int CountIndicator);
	void createSegment();
	static RECT getRect(int CountIndicator);
};

//width 430
//height 130 + 110 + 10