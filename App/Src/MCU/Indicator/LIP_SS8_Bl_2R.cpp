#include "LIP_SS8_Bl_2R.h"
#include "Message/Message.h"
#include "Indicator/ControlIndicatorSlot.h"
#include "Router/Router.h"
#include "ramdata.h"

#define LED_SIZE 8

std::vector<uint8_t> LIP_SS8_Bl_2R::getValue(){
    std::string data;
    ControlIndicatorSlot* controlSlot = Router::getInstance().getAppSlot("S1");
    data = controlSlot->getValueStr();
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID) || errorParsing){
        data = "0";
    }
    else{
        data = parametrControl.getValueStr();
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

void LIP_SS8_Bl_2R::ProcessMessage(TMessage* m){
    ++RAM_DATA.counter[1];
    Blink = !Blink;
}
