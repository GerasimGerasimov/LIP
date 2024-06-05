#pragma once

#include "Parameter.h"
#include <vector>

class TPrmList : public Parameter
{
private:
    char* MSU;
    float Scale;
    SpecialSignalAddr Addr;
    static const std::string SignalType;
    const std::string value(const TSlotHandlerArsg& args, const char* format);
    u8 getRawValue(const TSlotHandlerArsg& args);
    const std::string validation(const TSlotHandlerArsg& args);
public:
    TPrmList(ISignal::PropsPointers props);
    virtual std::string getMSU() override;
    virtual std::string getValue(const TSlotHandlerArsg& args, const char* format) override;
    std::vector<std::string> getList();
    std::vector<std::string> getList(const std::string& val, s16& ValueIndexInList);
    std::string getKeyByValue(const std::string& val);
    virtual const std::string getValueHex(std::string& src) override;
    virtual const std::string getRegHexAddr() override;
    virtual const std::string getWriteCmdType() override;
    virtual const std::string& getSignalType() override;
    virtual InternalMemAddress getInternalMemAddr() override;
    u8 getSizeByte() override;
};

