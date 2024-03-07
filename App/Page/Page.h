#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "Indicator/Indicator.h"
#include "Buffer.h"

class Page
{
private:
    std::vector<Indicator*> ListIndicators;
    Buffer bufferSender;
    Buffer bufferData;
    std::string str, str2, str3;
public:
    Page();
    ~Page();
    void update();
};

void updateDMA();