#pragma once

#include "Indicator.h"
#include "Slots/ParametrControlSlot.h"

class LIP_SS8 : public Indicator
{
private:
    ParametrControlSlot parametrControl;
public:
    LIP_SS8();
    ~LIP_SS8(){}
    std::vector<uint8_t> getValue() override;
    void setParameter(std::string& param) override;
    bool update() override;
    void stopSlot() override;
    void startSlot() override;
};

