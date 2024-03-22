#pragma once

#include "stm32f10x.h"

struct Intmash_Usart;
class MBmasterSlotType;

class STM32F10xUSART2
{
private:
	static Intmash_Usart* Usart;
	static MBmasterSlotType* Slot;

	void initUsart();
	void initTIM();
	void startNVIC();

public:
	STM32F10xUSART2(Intmash_Usart* Usart, MBmasterSlotType* slot);
	static Intmash_Usart* getUsart();
	static MBmasterSlotType* getSlot();
	void setCondition(u16 timeOut, u8* replyPtr);

};

