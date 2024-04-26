#pragma once

#include <stdint.h>
#include <string>
#include <vector>

class Parameter;

class Indicator
{
private:

public:
    virtual std::vector<uint8_t> getValue(std::string& data) = 0;
    uint8_t getDataSize();
    virtual ~Indicator(){};
    virtual void setParameter(std::string param);
protected:
    enum class Type {
        R,  //чтение
        RW  //чтение и запись
    };
    struct Props {
        std::string Device = "";
        std::string Section = "";
        std::string Name = "";
        Type type = Type::R;
        Parameter* resources;
    };
    
    uint8_t DataSize = 0;
};

