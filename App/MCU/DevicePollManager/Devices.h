#pragma once

#include "stm32f0xx.h"

#include <string>
#include <map>

//содержит все устройства, аналог IniSlotsProps
class Devices
{
    struct NetworkProps
    {
        std::string Source;
        std::string COM;
        u8 NetworkAddr;
    };
private:
    Devices();
    Devices(const Devices&) = delete;
    Devices& operator=(const Devices&) = delete;
    Devices(const Devices&&) = delete;
    Devices& operator=(const Devices&&) = delete;

    std::map<std::string, NetworkProps> ListDevise;
    NetworkProps getNetworkProps(std::string& dev);
public:
    static Devices& getInstance();

    u8 getDevNetWorkAddr(std::string position);
    std::string getSourceOfDev(std::string position);
};

