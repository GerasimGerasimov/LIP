#pragma once

#include "stm32f0xx.h"

struct InternalMemAddress
{
    s16 Offset;//-1 какая-то ошибка!
    u8 Lenght;
    s16 BitNumber;//-1 нет номера
};