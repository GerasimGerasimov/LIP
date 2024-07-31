#include "Indicator1Parametr.h"

//установить новый параметр
void Indicator1Parametr::setParameter(std::string& param){
    errorParsing = parametrControl.setParameter(param) ? false : true;
}

bool Indicator1Parametr::update(){
    if(errorParsing){
        return true;
    }
    if(parametrControl.isStateFlag(Slot::StateFlags::COMPLETE_READ)){
        return true;
    }
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID)){
        return true;
    }
    return false;
}

void Indicator1Parametr::stopSlot(){
    parametrControl.stopSlot();
}

void Indicator1Parametr::startSlot(){
    if(errorParsing){
        return;
    }
    parametrControl.startSlot();
}
