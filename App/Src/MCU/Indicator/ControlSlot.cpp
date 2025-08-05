#include "ControlSlot.h"

void ControlSlot::setParameter(std::string& param){
    errorParsing = parametrControl.setParameter(param) ? false : true;
}

bool ControlSlot::update(){
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

void ControlSlot::stopSlot(){
    parametrControl.stopSlot();
    updating = false;
}

void ControlSlot::startSlot(){
    if(errorParsing){
        return;
    }
    parametrControl.startSlot();
}

std::string ControlSlot::getValueStr(){
    return parametrControl.getValueStr();
}

bool ControlSlot::isStateFlag(Slot::StateFlags isFlag){
    return parametrControl.isStateFlag(isFlag);
}

bool ControlSlot::isErrorParsing(){
    return errorParsing;
}
