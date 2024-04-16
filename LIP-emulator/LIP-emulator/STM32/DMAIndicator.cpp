#include "DMAIndicator.h"

DMAIndicator::DMAIndicator() {}

void DMAIndicator::DMAstart(uint32_t BufferSize) {}

DMAIndicator& DMAIndicator::getInstance() {
	static DMAIndicator dma;
	return  dma;
}

void DMAIndicator::setMemoryBaseAddr(uint32_t addr) {}

void DMAIndicator::bringOutValue() {}
