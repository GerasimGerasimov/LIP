#include "stm32f10x.h"
#include "bastypes.h"
#pragma section = ".fdsection"
const struct TFLASH_DATA FLASH_DATA @ ".fdsection" =
{
	0x0401,                 //0x0000// device addres and baud rate for uart1 (modbus1);
	0x0401,                 //0x0002// device addres and baud rate for uart2 (modbus2);
        // Секция задания
    0x0263,                 //0x0004// Iz - 40%
    0x0685,                 //0x0006// Uz max - 150%
    0x0001,                 //0x000a// VA_fall_norm  - 5%/s
    0x000a,                 //0x000c// VA_rise 3град на синхру
    0x09c4,                 //0x000a// VA_fall_spark 1градус на 1 синхру
};


#pragma section = ".bkfdsection"
const struct TFLASH_DATA BKFLASH_DATA @ ".bkfdsection" = 
{
	0x0401,                 //0x0000// device addres and baud rate for uart1 (modbus1);
	0x0401,                 //0x0002// device addres and baud rate for uart2 (modbus2);
	    // Секция задания
    0x0263,                 //0x0004// Iz - 40%
    0x0685,                 //0x0006// Uz max - 150%
    0x0001,                 //0x000a// VA_fall_norm  - 5%/s
    0x000a,                 //0x000c// VA_rise 3град на синхру
    0x09c4,                 //0x000a// VA_fall_spark 1градус на 1 синхру
};


