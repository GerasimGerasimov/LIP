#pragma once

#include "BaseWindow.h"

#include <vector>
#include <memory>

class IndicatorContainer : public BaseWindow
{
protected:
	std::vector<std::unique_ptr<BaseWindow>> Indicators;
	RECT border;
	int indent;
	int widthIndicator; //длина дочернего индикатора
	int heightIndicator; //высота дочернего индикатора

	//в WinLIP_5Nx и WinLIPModule работает одинаково в Win7Segment останавливаются вызовы
	virtual std::vector<uint8_t> splitData(std::vector<uint8_t>::iterator& iterator, int byte);
public:

	IndicatorContainer(Parameter param);
	
	virtual void setData(std::vector<uint8_t>& vecData);
};

