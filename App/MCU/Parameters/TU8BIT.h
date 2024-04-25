#pragma once

#include "Parameter.h"

class TU8BIT : public Parameter
{
private:
	char* MSU;
	float Scale;
	SpecialSignalAddr Addr;
	static const std::string SignalType;
	const std::string value(const TSlotHandlerArsg& args, const char* format);
	u8 getRawValue(const TSlotHandlerArsg& args);
	const std::string validation(const TSlotHandlerArsg& args);
	u16 string2raw(std::string& src);
public:
	TU8BIT(ISignal::PropsPointers props);
	virtual std::string getMSU() override;
	virtual std::string getValue(const TSlotHandlerArsg& args, const char* format) override;
	virtual const std::string getValueHex(std::string& src) override;
	virtual const std::string getRegHexAddr() override;
	virtual const std::string getWriteCmdType() override;
	virtual const std::string& getSignalType() override;
	virtual InternalMemAddress getInternalMemAddr() override;
};

