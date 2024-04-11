#pragma once

#include "BaseWindow.h"
#include "Win7Segment.h"

#include <vector>
#include <memory>


//плата LIP.5Nx
class WinLIP_5Nx : public BaseWindow
{
private:
	std::vector<std::unique_ptr<Win7Segment>> Indicators;
	RECT border;
	int indent; // отступ между индикаторами
	int widthIndicator;
	int heightIndicator;
	const int count = 5;
public:
	WinLIP_5Nx(Parameter param);
	void createIndicator();
	
};

