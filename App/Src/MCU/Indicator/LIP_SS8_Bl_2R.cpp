#include "LIP_SS8_Bl_2R.h"
#include "Message/Message.h"
#include "Indicator/ControlIndicatorSlot.h"
#include "Router/Router.h"
#include "ini/parser.h"
#include "ramdata.h"

#define LED_SIZE 8

#define SWITCH_ON 0
#define SWITCH_ALARM 1
#define ISOL_WRN 2
#define ISOL_ALARM 3

#define SHIFT_GREEN 1
#define SHIFT_RED 2

#define SHIFT_BASE 1
#define SHIFT_ALARM 2

void LIP_SS8_Bl_2R::setRed(unsigned short indicator, unsigned short& result){
    result &= ~(SHIFT_GREEN << (indicator * 2));
    result |= SHIFT_RED << (indicator * 2);
}

void LIP_SS8_Bl_2R::setGreen(unsigned short indicator, unsigned short& result){
    result |= SHIFT_GREEN << (indicator * 2);
    result &= ~(SHIFT_RED << (indicator * 2));
}

void LIP_SS8_Bl_2R::setYellow(unsigned short indicator, unsigned short& result){
    result |= SHIFT_GREEN << (indicator * 2);
    result |= SHIFT_RED << (indicator * 2);
}

void LIP_SS8_Bl_2R::resetLED(unsigned short indicator, unsigned short& result){
    result &= ~(SHIFT_GREEN << (indicator * 2));
    result &= ~(SHIFT_RED << (indicator * 2));
}

LIP_SS8_Bl_2R::LIP_SS8_Bl_2R(){
    DataSize = 2;
}

std::vector<uint8_t> LIP_SS8_Bl_2R::getValue(){
    //TODO Разделить на функции
    std::string data;
     ControlIndicatorSlot* controlSlot = Slots[SWITCH_ON].Slot;
    if(controlSlot->isStateFlag(Slot::StateFlags::NO_VALID) || controlSlot->isErrorParsing()){
        data = "0";
    }
    else{
        data = controlSlot->getValueStr();
    }
    unsigned short number;
    unsigned short switchON = 0;
    unsigned short switchAlarm = 0;
    unsigned short res = 0;
    number = stoul(data);

    std::vector<std::pair<unsigned char, unsigned char>>* tempTagByte = &(Slots[SWITCH_ON].TagByte);  //Указатель для оптимизации
    for(int i = 0; i < tempTagByte->size(); ++i){
        if(number & (1 << (*tempTagByte)[i].first)){
            switchON |= (1 << (*tempTagByte)[i].second);
        }
    }

    controlSlot = Slots[SWITCH_ALARM].Slot;
    if(controlSlot->isStateFlag(Slot::StateFlags::NO_VALID) || controlSlot->isErrorParsing()){
        data = "0";
    }
    else{
        data = controlSlot->getValueStr();
    }

    number = stoul(data);

    tempTagByte = &(Slots[SWITCH_ALARM].TagByte);
    for(int i = 0; i < tempTagByte->size(); ++i){
        if(number & (1 << (*tempTagByte)[i].first)){
            switchAlarm |= (1 << (*tempTagByte)[i].second);
        }
    }

    unsigned short isolationData = 0;
    unsigned short isolationWarn = 0;
    unsigned short isolationAlarm = 0;
    if(Slots.size() > 2){
        controlSlot = Slots[ISOL_WRN].Slot;
        if(controlSlot->isStateFlag(Slot::StateFlags::NO_VALID) || controlSlot->isErrorParsing()){
            data = "0";
        }
        else{
            data = controlSlot->getValueStr();
        }
        isolationData = stoul(data);

        tempTagByte = &(Slots[ISOL_WRN].TagByte);
        for(int i = 0; i < tempTagByte->size(); ++ i){
            if(isolationData & (1 << (*tempTagByte)[i].first)){
                isolationWarn |= (1 << (*tempTagByte)[i].second);
            }
        }

        if(Slots.size() > 3){
            controlSlot = Slots[ISOL_ALARM].Slot;
            if(controlSlot->isStateFlag(Slot::StateFlags::NO_VALID) || controlSlot->isErrorParsing()){
                data = "0";
            }
            else{
                data = controlSlot->getValueStr();
            }
            isolationData = stoul(data);

            tempTagByte = &(Slots[ISOL_ALARM].TagByte);
            for(int i = 0; i < tempTagByte->size(); ++i){
                if(isolationData & (1 << (*tempTagByte)[i].first)){
                    isolationAlarm |= (1 << (*tempTagByte)[i].second);
                }
            }
        }
    }

    std::vector<uint8_t> result(DataSize);
    for(int i = 0; i < LED_SIZE; ++i){
        //Определение приоритетов
        if(switchAlarm & (1 << i)){
            Priority = PRIORITY::ALARM;
        }
        else if(isolationAlarm & (1 << i)){
            Priority = PRIORITY::ISOLATION_ALARM;
        }
        else if(isolationWarn & (1 << i)){
            Priority = PRIORITY::ISOLATION_WARNING;
        }
        else if(switchON & (1 << i)){
            Priority = PRIORITY::SWICH;
        }
        else{
            Priority = PRIORITY::BASE;
        }

        switch(Priority){
        case LIP_SS8_Bl_2R::PRIORITY::ALARM:
            //Мигание зелёным
            if(Blink){
                setGreen(i, res);
            }
            else{
                resetLED(i, res);
            }
            break;
        case LIP_SS8_Bl_2R::PRIORITY::ISOLATION_ALARM:
            if(Blink){
                setYellow(i, res);
            }
            else{
                resetLED(i, res);
            }
            break;
        case LIP_SS8_Bl_2R::PRIORITY::ISOLATION_WARNING:
            setYellow(i, res);
            break;
        case LIP_SS8_Bl_2R::PRIORITY::SWICH:
            setRed(i, res);
            break;
        case LIP_SS8_Bl_2R::PRIORITY::BASE:
            setGreen(i, res);
            break;
        }
    }
    result[1] = res;
    result[0] = res >> 8;
    return result;
}

bool LIP_SS8_Bl_2R::update(){
    //TODO перенести из Router::updateIndicatorSlots()
    return true;
}

void LIP_SS8_Bl_2R::ProcessMessage(TMessage* m){
    if(m->event == Event::TIMER){
        Blink = !Blink;
    }
}

void LIP_SS8_Bl_2R::setParameter(std::string& param){
    std::vector<std::string> tags = Parser::splitString(" ", param);
    for(auto& n : tags){
        IndicatorSlot indSlot;
        std::vector<std::string> spltTag = Parser::splitString(":", n);
        indSlot.Tag = spltTag[0];
        if(spltTag.size() > 1){
            std::vector<std::string> spltVecBytes = Parser::splitString("/", spltTag[1]);
            
            for(auto& j : spltVecBytes){
                std::vector<std::string> spltPair = Parser::splitString(".", j);
                std::pair<unsigned char, unsigned char> bytes = {stoi(spltPair[0]), stoi(spltPair[1])};
                indSlot.TagByte.push_back(bytes);
            }
        }
        Slots.push_back(indSlot);
    }
}

void LIP_SS8_Bl_2R::setAppSlots(std::map<std::string, ControlIndicatorSlot*>& indSlots){
    for(auto& n : Slots){
        if(indSlots.count(n.Tag)){
            n.Slot = indSlots.at(n.Tag);
        }
    }
}
