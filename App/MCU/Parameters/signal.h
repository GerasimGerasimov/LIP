#pragma once

#include <string>

class TSlotHandlerArsg;

class ISignal
{
public:
    struct PropsPointers
    {
        char* dev;
        char* pName;
        char* pComment;
        char* pType;
        char* pOptional;
    };

    ISignal(char* source, int scrLen);
    ISignal(PropsPointers props);
    virtual ~ISignal();
    virtual std::string getName();
    virtual std::string getValue(const TSlotHandlerArsg& args, const char* format);
    virtual std::string getValue(){ return ""; };
    virtual std::string getMSU();
    virtual const std::string& getSignalType();
    std::string getDefaultValue();
protected:
    char* Name;
    char* optional;
};

//typedef ISignal* pSignal;

