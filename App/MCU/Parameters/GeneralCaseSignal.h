#pragma once

#include "stm32f0xx.h"
#include "Parameter.h"



class GeneralCaseSignal : public Parameter
{
public:
enum class ReturnType : u8
{
	U = 0,
	S = 1,
	F = 2
};

struct RawReturn {
	GeneralCaseSignal::ReturnType type;
	union {
		float f;
		u32 i;
		s32 s;
	} raw;
};

	GeneralCaseSignal(ISignal::PropsPointers props);
	virtual std::string getMSU() override;
	virtual std::string getValue(const TSlotHandlerArsg& args, const char* format) override;
	virtual const std::string getRegHexAddr() override;
	virtual const std::string getWriteCmdType() override;
	u16 getAddr() override;
protected:
	virtual std::string value(const TSlotHandlerArsg& args, const char* format);
	virtual RawReturn getRawValue(const TSlotHandlerArsg& args);
	std::string validation(const TSlotHandlerArsg& args);
	char* MSU;
	s16 Addr;
	float Scale;
};

