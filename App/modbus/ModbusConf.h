#pragma once

#include "stm32f10x.h"
#include "STM32F10x_Intmash_USART.h"

//общий класс работы Modbus
class ModbusConf
{
protected:
	Intmash_Usart UART;
};

