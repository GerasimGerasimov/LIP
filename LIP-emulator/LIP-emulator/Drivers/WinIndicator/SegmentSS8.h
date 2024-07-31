#pragma once

#include "BaseObject.h"

class SegmentSS8 : public BaseObject
{
private:
    HBRUSH brushEnable; //включен
    HBRUSH brushDisable; //отключён
    HBRUSH brushDisableAlarm; //отключён по аварии
    HBRUSH brushOff; //не светит
    HDC hdc;
    void setColor(HBRUSH brush);
    void createSegment();
public:
    SegmentSS8(Parameter param);
    void setData(std::vector<uint8_t>& vecData) override;
};

