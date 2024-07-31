#pragma once

#include "Indicator.h"
#include "Slots/ParametrControlSlot.h"

#include <stdint.h>
#include <string>

//индикатор 5Nxx содержит слот, для чтения конкретного параметра
class LIP_5Nx : public Indicator
{
private:
    static const std::string parseErrorStr;
    static const std::string connectErrorStr;
    bool dot = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
    ParametrControlSlot parametrControl;
    void transformSizeSring(std::string& data);
public:
    LIP_5Nx();
    ~LIP_5Nx();
    std::vector<uint8_t> getValue() override;
    void setParameter(std::string& param) override;
    bool update() override;
    void stopSlot() override;
    void startSlot() override;
};

