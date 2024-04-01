#pragma once

#include "GeneralCaseSignal.h"

class TU16BIT : public GeneralCaseSignal
{
private:
	virtual GeneralCaseSignal::RawReturn getRawValue(const TSlotHandlerArsg& args) override;
	u16 string2raw(std::string& src);
public:
	TU16BIT(ISignal::PropsPointers props);
	virtual const std::string getValueHex(std::string& src) override;
	virtual InternalMemAddress getInternalMemAddr() override;
};

