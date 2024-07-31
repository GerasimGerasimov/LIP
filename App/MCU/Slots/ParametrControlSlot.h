#pragma once

#include "DevicePollManager/Slot.h"

#include <string>

class Parameter;

class ParametrControlSlot
{
protected:
    enum class Type
    {
        R,  //чтение
        RW  //чтение и запись
    };
    struct Props
    {
        std::string Device = "";
        std::string Section = "";
        std::string Name = "";
        Type type = Type::R;
        Parameter* resources = nullptr;
    };
private:
    Slot* slot;
    Props parameter;

    void clear();
    void createReadCmd();
    bool setIsignal();
public:
    ParametrControlSlot();
    ~ParametrControlSlot();
    std::string getValueStr();
    bool setParameter(std::string& param); //установить новую индикацию
    void stopSlot();
    void startSlot();
    void setFlag(Slot::StateFlags newFlag);
    void resetFlag(Slot::StateFlags delFlag);
    bool isStateFlag(Slot::StateFlags isFlag);
};

