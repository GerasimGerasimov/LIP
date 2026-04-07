#pragma once

#include "ParametrControlSlot.h"

class ParametrControlSlotXN : public ParametrControlSlot
{
private:
    enum class Type
    {
        R,  //чтение
        RW  //чтение и запись
    };
    Type type = Type::R;
    unsigned short numDecimal = 0;
protected:
    void setProps(std::vector<std::string>& page) override;
public:
    unsigned short getNumDecimal();
};

