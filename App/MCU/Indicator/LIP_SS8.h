#pragma once

#include "Indicator.h"

class Slot;

class LIP_SS8 : public Indicator
{
private:
    Slot* slot;
    bool setIsignal();
    void createReadCmd();
    void clear();
    std::string getValueStr();
    Props parameter;
public:
    LIP_SS8();
    ~LIP_SS8(){}
    std::vector<uint8_t> getValue() override;
    void setParameter(std::string param) override;
    bool update() override;
    void stopSlot() override;
    void startSlot() override;
};

