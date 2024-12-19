#pragma once

#include "Indicator1Parametr.h"

class LIP_SS8 : public Indicator1Parametr
{
private:
public:
    LIP_SS8();
    ~LIP_SS8(){}
    std::vector<uint8_t> getValue() override;
};

