#pragma once

#include <stdint.h>
#include <vector>
#include <string>

#include "Indicator/Indicator.h"

class Page
{
private:
    std::vector<Indicator*> List;
    std::string str, str2, str3;
public:
    Page();
    ~Page();
    void update();
};

