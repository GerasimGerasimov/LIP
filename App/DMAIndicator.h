#pragma once

#include <stdint.h>

class Buffer;

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
	void setMemoryBaseAddr(Buffer& buf);
	static void bringOutValue();
	void DMAstop();
	
};

