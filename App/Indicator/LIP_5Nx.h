#pragma once

#include <stdint.h>

#include "Indicator.h"

class LIP_5Nx : public Indicator
{
private:

public:
    LIP_5Nx();
    void setValue(uint8_t data[]);
};

