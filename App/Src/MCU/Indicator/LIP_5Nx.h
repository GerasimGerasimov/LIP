#pragma once

#include "Indicator1Parametr.h"

#include <stdint.h>
#include <string>

class LIP_XNx : public Indicator1Parametr
{
private:
    static const std::string parseErrorStr;
    static const std::string connectErrorStr;
    bool dot = false;
    static const char ASCIITable[96];
    uint8_t getChar(char symbol);
    void transformSizeSring(std::string& data);
    void transformNumDecimal(std::string& data);
public:
    LIP_XNx(uint8_t dataSize);
    ~LIP_XNx();
    std::vector<uint8_t> getValue() override;
};

