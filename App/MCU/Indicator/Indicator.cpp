#include "Indicator.h"

uint8_t Indicator::getDataSize(){
    return DataSize;
}

void Indicator::setParameter(std::string& param){}

void Indicator::setTypeAnode(bool anode){
    typeAnode = anode;
}
