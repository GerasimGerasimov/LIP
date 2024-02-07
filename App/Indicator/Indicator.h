#pragma once

#include <stdint.h>

class Indicator
{
private:

public:
    static void bringOutValue();
    virtual void setValue(const char* data){};
protected:
    uint8_t CountSegment;
};

