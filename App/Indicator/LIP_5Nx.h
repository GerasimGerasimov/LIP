#pragma once

#include <stdint.h>

#include "Indicator.h"

class LIP_5Nx : public Indicator
{
private:
    bool typeKathode = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
public:
    LIP_5Nx();
    void setValue(uint8_t data[]);
};

