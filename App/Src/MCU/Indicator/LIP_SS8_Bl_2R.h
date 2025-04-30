#pragma once

#include "Indicator.h"

#include <vector>
#include <string>

class ControlIndicatorSlot;

class LIP_SS8_Bl_2R : public Indicator
{
private:

    struct IndicatorSlot
    {
        std::string Tag;
        std::vector<std::pair<unsigned char, unsigned char>> TagByte;
        ControlIndicatorSlot* Slot = nullptr;
    };

    bool Blink = false;
    std::vector<IndicatorSlot> Slots;
public:
    LIP_SS8_Bl_2R();
    std::vector<uint8_t> getValue() override;
    bool update() override;
    void ProcessMessage(TMessage* m) override;
    void setParameter(std::string& param) override;
    void setAppSlots(std::map<std::string, ControlIndicatorSlot*>& indSlots) override;
};
