#pragma once

#include <vector>

enum class Status
{
    FILL,
    EMPTY
};

class Buffer
{
private:
    Status status;
public:
    std::vector<uint8_t> buffer;
    Buffer();
    Buffer& operator=(Buffer& buf);
    uint32_t getAddrBuffer();
    uint16_t getSize();
    Status getStatus();
    void swapStatus();
    void addData(std::vector<uint8_t>& data, uint16_t start, uint16_t end);
    void setSizeBuffer(uint8_t size);
};

