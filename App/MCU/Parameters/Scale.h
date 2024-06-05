#pragma once

#include "signal.h"

#include <vector>

class Scale : public ISignal
{
protected:
    int	ValueSize;
    std::string getValueFromList(std::vector<std::string> values);
public:
    struct Props
    {
        char* pKey;
        char* pValue;
        int	ValueSize;
    };

    Scale(char* source, int scrLen);
    Scale(Props props);
    virtual std::string getName() override;
    virtual std::string getValue() override;
};

