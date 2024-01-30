#pragma once

#include "stm32f10x.h"

class Indicator
{
private:

public:
    static void bringOutValue();
protected:
    u8 CountSegment;
};

