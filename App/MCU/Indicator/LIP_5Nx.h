#pragma once

#include "Indicator.h"

#include <stdint.h>
#include <string>

class Slot;

//индикатор 5Nxx содержит слот, для чтения конкретного параметра
class LIP_5Nx : public Indicator
{
private:
    static const std::string parseErrorStr;
    static const std::string connectErrorStr;
    bool typeKathode = false;
    bool dot = false;
    bool updating = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
    Slot* slot;
    void clear();
    bool setIsignal();
    void createReadCmd();
    Props parameter;
    std::string getValueStr();
    void transformSizeSring(std::string& data);
public:
    LIP_5Nx();
    std::vector<uint8_t> getValue() override;
    void setParameter(std::string param) override;
    bool update() override;
    void stopSlot() override;
    void startSlot() override;
};

