#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "Indicator/Indicator.h"
#include "Buffer/Buffer.h"

//управляет конкретными индикаторами отображения в данный момент
class Page
{
private:
    std::vector<Indicator*> ListIndicators;
    std::vector<std::string> Configuration;
    uint16_t sizeSegment = 0;
    void init();
    uint16_t countFillBuffer;
    Buffer* pageBuffer;
public:
    Page();
    ~Page();
    bool update();
    uint16_t getByteIndicators();
    void setIndication(std::string page);
    uint16_t getSizeSegment();
    void setBuffer(Buffer* newBuffer);
};

void updateDMA();