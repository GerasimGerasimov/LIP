#pragma once

#include "stm32f0xx.h"

struct TRAM_DATA
{
    u32 UsedMem; //4 байта
    u32 TotalMem; //4 байта
    u32 arena;
    u32 totalFreeSpace;
    //данные 32 бит
    u32 data32[8];
    //данные 16 бит
    u16 data[8];
    //счётчики
    u16 counter[8];
    u16 DI;
    u16 reserve;
    u16 reserve2;
    u16 reserve3;
};

extern TRAM_DATA RAM_DATA;