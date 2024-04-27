#pragma once

#include "framework.h"

#include <stdint.h>
#include <vector>

class Buffer;

class DMAIndicator
{
private:

	DMAIndicator();
	DMAIndicator(const DMAIndicator&) = delete;
	DMAIndicator& operator=(const DMAIndicator&) = delete;
	DMAIndicator(const DMAIndicator&&) = delete;
	DMAIndicator& operator=(const DMAIndicator&&) = delete;
	
	Buffer* buffer;
	std::vector<uint8_t> sendMessage;
public:
	void DMAstart(uint32_t BufferSize);
	void DMAstop();
	static DMAIndicator& getInstance();
	void setMemoryBaseAddr(Buffer& buf);
	static void bringOutValue();
	friend VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
};

