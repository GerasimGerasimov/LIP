#pragma once

#include "Slots/ParametrControlSlot.h"

class ControlSlot
{
private:
    ParametrControlSlot parametrControl;
    bool updating = false;
    bool errorParsing = false;
    bool completeRead = false;
    bool noValid = false;
public:
    void setParameter(std::string& param);
    bool update();
    void stopSlot();
    void startSlot();
    std::string getValueStr();
    std::vector<std::string>* getOption();
    bool isStateFlag(Slot::StateFlags isFlag);
    bool isErrorParsing();
    bool getcompleteRead();
    bool getnoValid();
    bool getUpdate();
};
