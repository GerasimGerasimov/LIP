#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "Indicator/Indicator.h"
#include "Buffer/Buffer.h"

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
    

};

void updateDMA();