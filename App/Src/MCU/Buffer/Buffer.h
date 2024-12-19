#pragma once

#include <vector>



class Buffer
{

public:
    enum class Status
    {
        FILL,
        EMPTY
    };
    std::vector<uint8_t> buffer;
    Buffer();
    uint32_t getAddrBuffer();
    uint16_t getSize();
    Status getStatus();
    void setFillStatus();
    void setEmptyStatus();
    void addData(std::vector<uint8_t>& data, uint16_t start, uint16_t end);
    void setSizeBuffer(uint8_t size);
private:
    Status status;
};

