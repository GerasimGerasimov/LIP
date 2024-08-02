#pragma once

#include "BaseWindow.h"

#include <vector>
#include <memory>
#include <map>

class WinDI : public BaseObject
{
private:
    std::vector<HWND> ButtonDI;

    void createButtonDI();
    const int countDI = 16;
    int widthButton;
    int heightButton;
public:
    WinDI(Parameter param);
    int getDIPush(HWND Param);
};

