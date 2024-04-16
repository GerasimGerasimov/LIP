#pragma once

#include "GeneralCaseSignal.h"

class TFloat : public GeneralCaseSignal
{
private:
	virtual std::string value(const TSlotHandlerArsg& args, const char* format) override;
	GeneralCaseSignal::RawReturn getRawValue(const TSlotHandlerArsg& args) override;
	u32 string2raw(std::string& src);
public:
	TFloat(ISignal::PropsPointers props);
	virtual InternalMemAddress getInternalMemAddr() override;
	virtual const std::string getValueHex(std::string& src) override;
};

