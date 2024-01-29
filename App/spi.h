#pragma once

#include <stdint.h>

class SPI
{
private:

public:
    SPI();
    void spi_send(uint8_t data);
    uint8_t spi_receve();
};

