#pragma once

#include "GeneralCaseSignal.h"

class TU32BIT : public GeneralCaseSignal
{
private:
	GeneralCaseSignal::RawReturn getRawValue(const TSlotHandlerArsg& args) override;
	u32 string2raw(std::string& src);
public:
	TU32BIT(ISignal::PropsPointers props);
	virtual const std::string getValueHex(std::string& src) override;
	virtual InternalMemAddress getInternalMemAddr() override;
	u8 getSizeByte() override;
};

