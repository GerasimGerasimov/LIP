#pragma once

#include "LIP_SS8.h"

class LIP_SS8_Bl_2R : public LIP_SS8
{
private:
    bool Blink = false;
public:
    std::vector<uint8_t> getValue() override;
    void ProcessMessage(TMessage* m) override;
};

