#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

class Parameter;
struct TMessage;
class ControlSlot;

class Indicator
{
private:

public:
    virtual std::vector<uint8_t> getValue() = 0;
    virtual bool update() = 0;
    uint8_t getDataSize();
    virtual ~Indicator(){};
    virtual void setParameter(std::string& param); //установить новую индикацию
    virtual void stopSlot(){};
    virtual void startSlot(){};
    void setTypeAnode(bool anode);
    virtual void ProcessMessage(TMessage* m){};
    virtual void setAppSlots(std::map<std::string, ControlSlot*>& indSlots){}
protected:
    bool updating = false;
    uint8_t DataSize = 0;
    bool typeAnode = false;
};

