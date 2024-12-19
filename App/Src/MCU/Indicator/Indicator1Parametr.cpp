#include "Indicator1Parametr.h"

//установить новый параметр
void Indicator1Parametr::setParameter(std::string& param){
    errorParsing = parametrControl.setParameter(param) ? false : true;
}

bool Indicator1Parametr::update(){
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

void Indicator1Parametr::stopSlot(){
    parametrControl.stopSlot();
    updating = false;
}

void Indicator1Parametr::startSlot(){
    if(errorParsing){
        return;
    }
    parametrControl.startSlot();
}
