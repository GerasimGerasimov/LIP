#pragma once

#include "stm32f10x.h"
#include "signal.h"

#include <string>

struct  SpecialSignalAddr {
	s16 Addr;
	u16 Option;
};

struct InternalMemAddress;

class Parameter : public ISignal
{
protected:
	char* Comment;
	char* strAddr;
public:
	Parameter(char* source, int scrLen);
	Parameter(ISignal::PropsPointers props);
	virtual const std::string getValueHex(std::string& src);
	virtual const std::string getRegHexAddr();
	virtual const std::string getWriteCmdType();
	virtual InternalMemAddress getInternalMemAddr();
	virtual u8 getSizeByte();
};

