#pragma once

#include "Indicator.h"

#include <stdint.h>
#include <string>

class Slot;

//индикатор 5Nxx содержит слот, для чтения конкретного параметра
class LIP_5Nx : public Indicator
{
private:


    bool typeKathode = false;
    bool dot = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
    Slot* slot;
    void clear();
public:
    LIP_5Nx();
    std::vector<uint8_t> getValue(std::string & data) override;
    void setParameter(std::string param) override;
};

