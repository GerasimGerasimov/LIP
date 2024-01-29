#pragma once

#include <stdint.h>

#include "stm32f10x.h"

class LIP_5Nx
{
private:
    const u8 countChar = 5;
public:
    LIP_5Nx();
    void setVal();
};

