#pragma once

#include "IndicatorContainer.h"
#include "Win7Segment.h"

#include <vector>

//плата LIP.5Nx
class WinLIP_XNx : public IndicatorContainer
{
private:
    static const int desirWidth = 310;
    static const int desirHeight = 130;
    int count;
public:
    WinLIP_XNx(Parameter param, uint8_t dataSize);
    void createIndicator();
    int getByteIndication() override;
    static int getDesirWidth();
    static int getDesirHeight();
};

