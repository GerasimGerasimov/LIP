#include "id.h"
#include "Resources/InternalResources.h"
#include "flashdata.h"

#include <string>

std::string DeviceID;

void IDinit(const char* idText){

    // DeviceID = InternalResources::getInstance().getItemStringByName("ID");
    char s[10];
    sprintf(s, "%08u", FLASH_DATA.SerialNumber);
    DeviceID = s;
    DeviceID += idText;
}

u8 GetDeviceIDLength(){
    return DeviceID.size();
}

const char* getID(){
    return DeviceID.c_str();
}