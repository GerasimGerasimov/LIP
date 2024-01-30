#pragma once

#include "stm32f10x.h"

class Indicator
{
private:

public:
    static void bringOutValue();
protected:
    uint8_t CountSegment;
};

