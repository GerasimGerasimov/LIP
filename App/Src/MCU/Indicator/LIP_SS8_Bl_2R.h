#pragma once

#include "Indicator.h"

#include <vector>
#include <string>
#include <map>
#include <functional>

class ControlSlot;

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

    enum class COLOR
    {
        BLACK,
        RED,
        GREEN,
        YELLOW
    };

    struct ColorSettingLED
    {
        COLOR CurrentColor = COLOR::BLACK;
        bool Blink = false; //Мигание
    };

    struct IndicatorSlot
    {
        std::string Tag;
        //Битовые параметры:  от куда брать,  куда ложить
        std::vector<std::pair<unsigned char, unsigned char>> TagByte;
        ControlSlot* SlotControl = nullptr;
    };

    bool Blink = false;
    bool updating = false;
    ColorSettingLED BaseSettingIndicator; //Настройки цвет, который отображается, когда нет сигналов
    std::map<COLOR, std::function<void(unsigned short, unsigned short&)>> ColorFunction;
    std::vector<std::vector<IndicatorSlot>> SlotsControlList; //Список слотов<Информация о обработке светодиодов для слота>
    PRIORITY Priority = PRIORITY::BASE; //Приоритет цвета для 1 светодиода
    void setColorFunction();
    void setBaseSetting(std::vector<std::string>& ConfigOption);
    COLOR getColorByChar(char symbol);
    static void setRed(unsigned short indicator, unsigned short& result);
    static void setGreen(unsigned short indicator, unsigned short& result);
    static void setYellow(unsigned short indicator, unsigned short& result);
    static void resetLED(unsigned short indicator, unsigned short& result);
    void clear();
    unsigned short updateColorLedState(std::vector<IndicatorSlot>* tagsFunction);
    unsigned short setColorPriorityLED(unsigned short switchLED[]);
    void setPriorityLED(unsigned short switchLED[], int LED);
public:
    LIP_SS8_Bl_2R(std::vector<std::string>& ConfigOption);
    std::vector<uint8_t> getValue() override;
    bool update() override;
    void ProcessMessage(TMessage* m) override;
    void setParameter(std::string& param) override;
    void setAppSlots(std::map<std::string, ControlSlot*>& indSlots) override;
    void stopSlot() override;
};
