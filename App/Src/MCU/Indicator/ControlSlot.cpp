#include "ControlSlot.h"

#include "ramdata.h"

void ControlSlot::setParameter(std::string& param){
    errorParsing = parametrControl.setParameter(param) ? false : true;
}

bool ControlSlot::update(){
    
    //ошибка парсинга
    if(errorParsing){
        return true;
    }
    //прочитали данные
    if(parametrControl.isStateFlag(Slot::StateFlags::COMPLETE_READ)){

        return true;
    }
    //нет связи
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID)){

        return true;
    }
    return false;
}

void ControlSlot::stopSlot(){
    parametrControl.stopSlot();
    //updating = false;
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

std::vector<std::string>* ControlSlot::getOption(){
    return parametrControl.getOption();
}

bool ControlSlot::isStateFlag(Slot::StateFlags isFlag){
    return parametrControl.isStateFlag(isFlag);
}

bool ControlSlot::isErrorParsing(){
    return errorParsing;
}
