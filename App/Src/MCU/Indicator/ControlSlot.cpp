#include "ControlSlot.h"

#include "ramdata.h"

bool ControlSlot::setParameter(std::string& param){
    errorParsing = parametrControl.setParameter(param) ? false : true;
    return (!errorParsing);
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
        completeRead = true;
        return true;
    }
    else{
        completeRead = false;
    }
    //нет связи
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID)){
        updating = true;
        noValid = true;
        return true;
    }
    else{
        noValid = false;
    }
    updating = false;
    return false;
}

void ControlSlot::stopSlot(){
    updating = false;
    parametrControl.stopSlot();
    noValid = false;
    completeRead = false;
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

bool ControlSlot::getcompleteRead(){
    return completeRead;
}

bool ControlSlot::getnoValid(){
    return noValid;
}

bool ControlSlot::getUpdate() {
    return updating;
}
