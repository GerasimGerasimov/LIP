#pragma once

#include <stdint.h>
#include <string>

class Indicator
{
private:

public:
    static void bringOutValue();
    virtual void setValue(std::string & data){};
    uint8_t getDataSize();
protected:
    uint8_t DataSize;
};

