#pragma once

#include "BaseObject.h"

class BaseWindow : public BaseObject
{
protected:
    void createIndicatorWindow();
public:
    BaseWindow(Parameter param);
};

