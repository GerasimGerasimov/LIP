#pragma once

#include "Indicator.h"

#include <vector>
#include <string>

class ControlIndicatorSlot;

class LIP_SS8_Bl_2R : public Indicator
{
private:

    enum class PRIORITY
    {
        ALARM,
        ISOLATION_WARNING,
        ISOLATION_ALARM,
        SWICH,
        BASE
    };

    struct IndicatorSlot
    {
        std::string Tag;
        //Битовые параметры:  от куда брать,  куда ложить
        std::vector<std::pair<unsigned char, unsigned char>> TagByte;
        ControlIndicatorSlot* Slot = nullptr;
    };

    bool Blink = false;
    std::vector<std::vector<IndicatorSlot>> Slots;
    PRIORITY Priority = PRIORITY::BASE;
    void setRed(unsigned short indicator, unsigned short& result);
    void setGreen(unsigned short indicator, unsigned short& result);
    void setYellow(unsigned short indicator, unsigned short& result);
    void resetLED(unsigned short indicator, unsigned short& result);
public:
    LIP_SS8_Bl_2R();
    std::vector<uint8_t> getValue() override;
    bool update() override;
    void ProcessMessage(TMessage* m) override;
    void setParameter(std::string& param) override;
    void setAppSlots(std::map<std::string, ControlIndicatorSlot*>& indSlots) override;
};
