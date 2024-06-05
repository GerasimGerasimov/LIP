#pragma once

#include "BaseWindow.h"
#include "Segment.h"
#include "IndicatorContainer.h"

#include <vector>
#include <memory>

// 7-сегментный индикатор
class Win7Segment : public IndicatorContainer
{
private:
    enum class Type
    {
        ANODE,
        KATHODE
    };

    static const Type type = Type::ANODE;
    std::vector<RECT> LocationSegments;

    void fillLocationSegment();
    int segmentWeight;
    std::vector<uint8_t> splitData(std::vector<uint8_t>::iterator& iterator, int byte) override;
public:
    Win7Segment(Parameter param);
    void setData(std::vector<uint8_t>& vecData) override;
    //void on1(HDC hdc);
    void createSegment();
    int getByteIndication() override;
};

/*
    порядок расположения сегментов
        1
     _______
    |		|
  6 |		| 2
    |_______|
    |	7	|
  5 |		| 3
    |_______|  .8
        4

*/