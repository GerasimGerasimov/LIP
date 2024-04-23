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

    void init();
public:
    Page();
    ~Page();
    void update();
    uint16_t getByteIndicators();
    void setIndication(std::string page);

};

void updateDMA();