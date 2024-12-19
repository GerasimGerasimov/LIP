#pragma once

#include "stm32f0xx.h"

struct SpecialSignalAddr;

namespace ParametersUtils
{

    s16 getByteOffsetFromSlahedAddrStr(char* addr);
    SpecialSignalAddr getSpecialAddrForBit(char* addr);
    SpecialSignalAddr getSpecialAddrForByte(char* addr);
    bool isAddrInvalid(s16 Addr);
    bool isAddrValid(s16 Addr);
};

