#pragma once

#include "stm32f10x.h"

struct Intmash_Usart;
class MBmasterSlotType;

class STM32F10xUSART2
{
private:
	Intmash_Usart* Usart;


	void initUsart();
	void initTIM();
	void startNVIC();
public:
	void SetTimer(u16 delay);
	void StopTimer();
	STM32F10xUSART2(Intmash_Usart* Usart);


};

