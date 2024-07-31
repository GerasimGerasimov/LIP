#include "LIP_SS8.h"

LIP_SS8::LIP_SS8(){
    DataSize = 2;
}

std::vector<uint8_t> LIP_SS8::getValue(){
    std::string data;
    if(parametrControl.isStateFlag(Slot::StateFlags::NO_VALID) || errorParsing){
        data = "0";
    }
    else{
        data = parametrControl.getValueStr();
    }
    unsigned short number;
    number = stoul(data);
    std::vector<uint8_t> result(DataSize);
    result[0] = number;
    result[1] = number >> 8;
    return result;
}
