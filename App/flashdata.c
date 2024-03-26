#include "stm32f10x.h"
#include "bastypes.h"
#pragma section = ".fdsection"
const struct TFLASH_DATA FLASH_DATA @ ".fdsection" =
{
	0x0401,                 //0x0000// device addres and baud rate for uart1 (modbus1);
	0x0301,                 //0x0002// device addres and baud rate for uart2 (modbus2);
    
    // Секция задания
    0x0263,                 //0x0004// Iz - 40%
    0x0685,                 //0x0006// Uz max - 150%
    0x0001,                 //0x000a// VA_fall_norm  - 5%/s
    0x000a,                 //0x000c// VA_rise 3град на синхру
    0x09c4,                 //0x000a// VA_fall_spark 1градус на 1 синхру

    // Секция ПИ-регулятора
    0x0006,                 //0x000e// Ky напряжение
    0x0006,                 //0x000e// Ky ток
    // Секция Защиты
    0x0fff,                 //0x0012// Max Ish
    0x08f4,                 //0x0014// Max Iload -150%
    0x0008,                 //0x0036//Iload_OPC
    0x0685,                 //0x0038//Uload_OPC   
    0x0099,                 //0x003a//Iload_SHC
    0x002a,                 //0x003c//Uload_SHC
    0x0064,         //0x003c// проверка на частые пробои, сколько единиц выставлено в течение 0.5с ?

    // Секция Встряхивателя
    0x01f4,                 //0x0016// shacker periodical  600... sec = 0x0258
    0x0064,                 //0x0018// shacker work time 1-5 sec = 0x0005

    // Секция датчика искрового разряда
    0x0003,                 //0x001e//  сколько импульсов синхры пропустить после искры, перед запуском тиристоров. Не меньше 1!
    0x00d0,                 //0x0020// Treshold Load voltage for ESpark
    0x0096,                 //0x0022// Treshold Load Current for ESpark - 23 %
    0x0116,   //чувствительность искры
    
    // Секция тиристорного регулятора
    0x24E4,                 //0x0028// 170 /175 град max thyristors angle 1тик=1мкс=(18/1000)градуса 175=9722 тика
    0x0116,                 //0x002a// 5 град  min thyristors angle 278тиков
    0x0683,                 //0x002c// impulse width 1тик=1мкс=(18/1000)градуса //30 градусов пока что-1667
    0x006B,                 //0x002e// скважность высокочастотного наполнения имульсов тиристоров - 30%
    
    // Секция нулей АЦП
    0x056e, //коэффициент для калибровки Vload
    0x056e,     //Iload
    0x056e,     //Ish
    
    0x0000,                 //0x0030// Vload offset
    0x0000,                 //0x0032// Iload offset
    0x0000,                 //0x0034// Ish   offset
    
    0x0827,
    0x001a,
    0x0064,
    0x0172,
    
   // 9560 //FTlim
    
};


#pragma section = ".bkfdsection"
const struct TFLASH_DATA BKFLASH_DATA @ ".bkfdsection" = 
{
	0x0401,                 //0x0000// device addres and baud rate for uart1 (modbus1);
	0x0301,                 //0x0002// device addres and baud rate for uart2 (modbus2);
    
    // Секция задания
    0x0263,                 //0x0004// Iz - 40%
    0x0685,                 //0x0006// Uz max - 150%
    0x0001,                 //0x000a// VA_fall_norm  - 5%/s
    0x000a,                 //0x000c// VA_rise 3град на синхру
    0x09c4,                 //0x000a// VA_fall_spark 1градус на 1 синхру

    // Секция ПИ-регулятора
    0x0006,                 //0x000e// Ky напряжение
    0x0006,                 //0x000e// Ky ток
    // Секция Защиты
    0x0fff,                 //0x0012// Max Ish
    0x08f4,                 //0x0014// Max Iload -150%
    0x0008,                 //0x0036//Iload_OPC
    0x0685,                 //0x0038//Uload_OPC   
    0x0099,                 //0x003a//Iload_SHC
    0x002a,                 //0x003c//Uload_SHC
    0x0064,         //0x003c// проверка на частые пробои, сколько единиц выставлено в течение 0.5с ?

    // Секция Встряхивателя
    0x01f4,                 //0x0016// shacker periodical  600... sec = 0x0258
    0x0064,                 //0x0018// shacker work time 1-5 sec = 0x0005

    // Секция датчика искрового разряда
    0x0003,                 //0x001e//  сколько импульсов синхры пропустить после искры, перед запуском тиристоров. Не меньше 1!
    0x00d0,                 //0x0020// Treshold Load voltage for ESpark
    0x0096,                 //0x0022// Treshold Load Current for ESpark - 23 %
    0x0116,   //чувствительность искры
    
    // Секция тиристорного регулятора
    0x25fa,                 //0x0028// 175 град max thyristors angle 1тик=1мкс=(18/1000)градуса 175=9722 тика
    0x0116,                 //0x002a// 5 град  min thyristors angle 278тиков
    0x0683,                 //0x002c// impulse width 1тик=1мкс=(18/1000)градуса //30 градусов пока что-1667
    0x006B,                 //0x002e// скважность высокочастотного наполнения имульсов тиристоров - 30%
    
    // Секция нулей АЦП
    0x056e, //коэффициент для калибровки Vload
    0x056e,     //Iload
    0x056e,     //Ish
    
    0x0000,                 //0x0030// Vload offset
    0x0000,                 //0x0032// Iload offset
    0x0000,                 //0x0034// Ish   offset
    
    0x0827,
    0x001a,
    0x0064,
    0x0172,
    
   // 9560 //FTlim
    
    
};


