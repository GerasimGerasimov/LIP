#pragma once

#include <stdint.h>

class SPI
{
private:
    SPI(){};
    SPI(const SPI&) = delete;
    SPI& operator=(const SPI&) = delete;
    SPI(const SPI&&) = delete;
    SPI& operator=(const SPI&&) = delete;
public:
    void spi_send(uint8_t data);
    uint8_t spi_receve();
    static SPI& getInstance();
};

