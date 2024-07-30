#pragma once

#include "IndicatorContainer.h"
#include "WinLIP_5Nx.h"

#include <string>

class WinLIPModule : public IndicatorContainer
{
private:
    std::vector<std::string> Configuration;
    void insertRectParamHeight(BaseWindow::Parameter& param);
public:
    WinLIPModule(Parameter param, std::vector<std::string> Config, RECT newBorder);
    void createSegmentHeight();
};

//width 430
//height 130 + 110 + 10