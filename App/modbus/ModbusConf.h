#pragma once

#include "stm32f0xx.h"
#include "STM32F0xx_Intmash_USART.h"

//общий класс работы Modbus
class ModbusConf
{
protected:
	Intmash_Usart UART;
};

