#include "ControlIndicatorSlot.h"

void ControlIndicatorSlot::setParameter(std::string& param){
    errorParsing = parametrControl.setParameter(param) ? false : true;
}

bool ControlIndicatorSlot::update(){
    if(updating){
        return false;
    }
    //ошибка парсинга
    if(errorParsing){
        updating = true;
        return true;
    }
    //прочитали данные
    if(parametrControl.isStateFlag(Slot::StateFlags::COMPLETE_READ)){
        updating = true;
        return true;
    }
    //нет связи
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID)){
        updating = true;
        return true;
    }
    updating = false;
    return false;
}

void ControlIndicatorSlot::stopSlot(){
    parametrControl.stopSlot();
    updating = false;
}

void ControlIndicatorSlot::startSlot(){
    if(errorParsing){
        return;
    }
    parametrControl.startSlot();
}

std::string ControlIndicatorSlot::getValueStr(){
    return parametrControl.getValueStr();
}

bool ControlIndicatorSlot::isStateFlag(Slot::StateFlags isFlag){
    return parametrControl.isStateFlag(isFlag);
}

bool ControlIndicatorSlot::isErrorParsing(){
    return errorParsing;
}
