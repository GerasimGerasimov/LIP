#pragma once

#include "BaseWindow.h"

#include <vector>
#include <memory>

class IndicatorContainer : public BaseWindow
{
protected:
	std::vector<std::unique_ptr<BaseObject>> Indicators;
	RECT border;
	int indent;
	int widthIndicator; //длина дочернего индикатора
	int heightIndicator; //высота дочернего индикатора
public:
	IndicatorContainer(Parameter param);

};

