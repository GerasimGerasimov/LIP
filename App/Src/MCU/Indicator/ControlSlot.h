#pragma once

#include "Slots/ParametrControlSlot.h"

class ControlSlot
{
private:
    ParametrControlSlot parametrControl;
    bool updating = false;
    bool errorParsing = false;
public:
    void setParameter(std::string& param);
    bool update();
    void stopSlot();
    void startSlot();
    std::string getValueStr();
    bool isStateFlag(Slot::StateFlags isFlag);
    bool isErrorParsing();
};
