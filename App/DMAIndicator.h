#pragma once

#include <stdint.h>

class DMAIndicator
{
private:
	
	DMAIndicator();
	DMAIndicator(const DMAIndicator&) = delete;
	DMAIndicator& operator=(const DMAIndicator&) = delete;
	DMAIndicator(const DMAIndicator&&) = delete;
	DMAIndicator& operator=(const DMAIndicator&&) = delete;
public:
	void DMAstart(uint32_t BufferSize);
	static DMAIndicator& getInstance();
	void setMemoryBaseAddr(uint32_t addr);
};

