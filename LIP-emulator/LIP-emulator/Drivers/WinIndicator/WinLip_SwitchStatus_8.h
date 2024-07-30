#pragma once

#include "IndicatorContainer.h"

//TODO пробное название
class WinLip_SwitchStatus_8 : public IndicatorContainer
{
private:
    const int count = 8;//каждая лампочка - 2 бита
    const int line = 2;
public:
    WinLip_SwitchStatus_8(Parameter param);
    void createIndicator();
    int getByteIndication() override;
    void setData(std::vector<uint8_t>& vecData) override;
};

