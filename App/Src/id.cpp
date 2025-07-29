#include "id.h"
#include "Resources/InternalResources.h"

#include <string>

std::string DeviceID;

void IDinit(const char* idText){

    DeviceID = InternalResources::getInstance().getItemStringByName("ID");;
    DeviceID += idText;
}

u8 GetDeviceIDLength(){
    return DeviceID.size();
}

const char* getID(){
    return DeviceID.c_str();
}