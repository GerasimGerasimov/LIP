#include "LIP_SS8_Bl_2R.h"
#include "Message/Message.h"
#include "Indicator/ControlIndicatorSlot.h"
#include "Router/Router.h"
#include "ini/parser.h"
#include "ramdata.h"

#define LED_SIZE 8
#define ON_OFF 0
#define ISOLATION_CONTROL 1

LIP_SS8_Bl_2R::LIP_SS8_Bl_2R(){
    DataSize = 2;
}

std::vector<uint8_t> LIP_SS8_Bl_2R::getValue(){
    std::string data;
     ControlIndicatorSlot* controlSlot = Slots[ON_OFF].Slot;
    if(controlSlot->isStateFlag(Slot::StateFlags::NO_VALID) || controlSlot->isErrorParsing()){
        data = "0";
    }
    else{
        data = controlSlot->getValueStr();
    }
    unsigned short number;
    unsigned short res = 0;
    number = stoul(data);
    std::vector<uint8_t> result(DataSize);
    for(int i = 0; i < LED_SIZE * 2; ++i){
        if(i % 2 == 0){
            if(number & (1 << i)){
                res |= 2 << i;
            }
            else{
                res |= 1 << i;
            }
        }
        else{
            if(number & (1 << i)){
                res &= ~(2 << (i - 1));
                if(Blink){
                    res |= (1 << (i - 1));
                }
                else{
                    res &= ~(1 << (i - 1));
                    res &= ~(2 << (i - 1));
                }
            }
        }
    }
    //for(int i = 1; i < LED_SIZE * 2; i += 2){
    //    if(number & (1 << i)){
    //        res |= 2 << i;
    //        res &= ~(1 << i);
    //    }
    //}
    result[1] = res;
    result[0] = res >> 8;
    return result;
}

bool LIP_SS8_Bl_2R::update(){
    //TODO перенести из Router::updateIndicatorSlots()
    return true;
}

void LIP_SS8_Bl_2R::ProcessMessage(TMessage* m){
    Blink = !Blink;
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
