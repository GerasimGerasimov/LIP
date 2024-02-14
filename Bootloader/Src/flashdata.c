#include "stm32f10x.h"
#include "bastypes.h"
#pragma section = ".fdsection"
const struct TFLASH_DATA FLASH_DATA @ ".fdsection" =
{
	0x0401,                 //0x0000// device addres and baud rate for uart1 (modbus1);
	0x0401,                 //0x0002// device addres and baud rate for uart2 (modbus2);

};


#pragma section = ".bkfdsection"
const struct TFLASH_DATA BKFLASH_DATA @ ".bkfdsection" = 
{
	0x0401,                 //0x0000// device addres and baud rate for uart1 (modbus1);
	0x0401,                 //0x0002// device addres and baud rate for uart2 (modbus2);

};


