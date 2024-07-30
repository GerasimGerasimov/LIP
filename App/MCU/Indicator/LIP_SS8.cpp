#include "LIP_SS8.h"

LIP_SS8::LIP_SS8(){
    DataSize = 2;
}

std::vector<uint8_t> LIP_SS8::getValue(){
    std::vector<uint8_t> result(DataSize);
    result[0] = 63;
    result[1] = 192;
    return result;
}

bool LIP_SS8::update(){
    return true;
}
