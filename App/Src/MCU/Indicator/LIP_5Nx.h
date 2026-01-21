#pragma once

#include "Indicator.h"
#include "Indicator/ControlSlot.h"

#include <stdint.h>
#include <string>

class LIP_XNx : public Indicator
{
private:
    enum class Type
    {
        R,  //чтение
        RW  //чтение и запись
    };
    Type type = Type::R;
    unsigned short numDecimal = 0;

    ControlSlot parametrControl;
    static const std::string parseErrorStr;
    static const std::string connectErrorStr;
    bool dot = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
    void transformSizeSring(std::string& data);
    void transformNumDecimal(std::string& data);
public:
    LIP_XNx(std::vector<std::string>& ConfigOption);
    ~LIP_XNx();
    std::vector<uint8_t> getValue() override;
    bool update() override;
    void setParameter(std::string& param);
    void stopSlot() override;
    void startSlot() override;
};

