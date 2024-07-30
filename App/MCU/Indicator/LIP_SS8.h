#pragma once

#include "Indicator.h"

class LIP_SS8 : public Indicator
{
public:
    LIP_SS8();
    ~LIP_SS8(){}
    std::vector<uint8_t> getValue() override;
    bool update() override;
};

