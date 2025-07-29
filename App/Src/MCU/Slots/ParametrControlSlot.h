#pragma once

#include "DevicePollManager/Slot.h"

#include <string>

class Parameter;

class ParametrControlSlot
{
protected:
    struct Props
    {
        std::string Device = "";
        std::string Section = "";
        std::string Name = "";
        Parameter* resources = nullptr;
    };

    virtual void setProps(std::vector<std::string>& page);
private:
    Slot* slot;
    Props parameter;

    void clear();
    void createReadCmd();
    bool setIsignal();
public:
    ParametrControlSlot();
    virtual ~ParametrControlSlot();
    std::string getValueStr();
    bool setParameter(std::string& param); //установить новую индикацию
    void stopSlot();
    void startSlot();
    void setFlag(Slot::StateFlags newFlag);
    void resetFlag(Slot::StateFlags delFlag);
    bool isStateFlag(Slot::StateFlags isFlag);
};

