#pragma once

#include "Parameter.h"

class TBit : public Parameter
{
private:
    SpecialSignalAddr Addr;
    const std::string value(const TSlotHandlerArsg& args);
    bool getRawValue(const TSlotHandlerArsg& args);
    const std::string validation(const TSlotHandlerArsg& args);
    static const std::string SignalType;
    static const std::array<u16, 16> Mask2BitNumber;
public:
    TBit(ISignal::PropsPointers props);
    virtual std::string getValue(const TSlotHandlerArsg& args, const char* format) override;
    virtual const std::string getValueHex(std::string& src) override;
    virtual const std::string getRegHexAddr() override;
    virtual const std::string getWriteCmdType() override;
    virtual const std::string& getSignalType() override;
    virtual InternalMemAddress getInternalMemAddr() override;
};

