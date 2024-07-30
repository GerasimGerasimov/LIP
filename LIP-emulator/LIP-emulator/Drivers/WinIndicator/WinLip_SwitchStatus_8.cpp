#include "WinLip_SwitchStatus_8.h"

WinLip_SwitchStatus_8::WinLip_SwitchStatus_8(Parameter param) : IndicatorContainer(param){

}

void WinLip_SwitchStatus_8::createIndicator(){}

int WinLip_SwitchStatus_8::getByteIndication(){
    return count >> 2; // count / 4
}
