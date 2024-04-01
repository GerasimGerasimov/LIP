#pragma once

#include "stm32f10x.h"

struct TSlotHandlerArsg{
	u8* InputBuf;//буфер сообщения для приёма сообщений
	u8 InputBufValidBytes;
	u16 StartAddrOffset;
	u16 LastAddrOffset;
} ;

