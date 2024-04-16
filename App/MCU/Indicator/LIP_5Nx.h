#pragma once

#include <stdint.h>
#include <string>

#include "Indicator.h"

class LIP_5Nx : public Indicator
{
private:
    bool typeKathode = false;
    bool dot = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
public:
    LIP_5Nx();
    virtual void setValue(std::string& data) override;
    virtual std::vector<uint8_t> getValue(std::string & data) override;
    virtual ~LIP_5Nx(){};
};

