#pragma once

#include "GeneralCaseSignal.h"

class TS16BIT : public GeneralCaseSignal
{
private:
	virtual GeneralCaseSignal::RawReturn getRawValue(const TSlotHandlerArsg& args) override;
public:
	TS16BIT(ISignal::PropsPointers props);
	virtual InternalMemAddress getInternalMemAddr() override;
};

