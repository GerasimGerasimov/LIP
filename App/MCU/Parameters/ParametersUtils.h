#pragma once

#include "stm32f10x.h"

struct SpecialSignalAddr;

namespace ParametersUtils {

	s16 getByteOffsetFromSlahedAddrStr(const char* addr);
	SpecialSignalAddr getSpecialAddrForBit(const char* addr);
	SpecialSignalAddr getSpecialAddrForByte(const char* addr);
	bool isAddrInvalid(s16 Addr);
	bool isAddrValid(s16 Addr);
};

