#pragma once

#include <stdint.h>
#include <string>
#include <vector>

class Indicator
{
private:

public:
    static void bringOutValue();
    virtual void setValue(std::string & data){};
    virtual std::vector<uint8_t> getValue(std::string& data) = 0;
    uint8_t getDataSize();
    virtual ~Indicator(){};
protected:
    uint8_t DataSize;
};

