#pragma once

#include "IndicatorContainer.h"
#include "Win7Segment.h"

#include <vector>

//плата LIP.5Nx
class WinLIP_5Nx : public IndicatorContainer
{
private:
    static const int desirWidth = 310;
    static const int desirHeight = 90;
    const int count = 5;
public:
    WinLIP_5Nx(Parameter param);
    void createIndicator();
    int getByteIndication() override;
    static int getDesirWidth();
    static int getDesirHeight();
};

