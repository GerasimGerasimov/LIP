#include "DMAIndicator.h"
#include "Page/Page.h"
#include "MainWindow.h"

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime) {
	updateDMA();
	//for (int i = 0; i < DMAIndicator::getInstance().sendMessage.size(); ++i) {
		//DMAIndicator::getInstance().sendMessage[i] = DMAIndicator::getInstance().BaseAddr[i];
	//}
	MainWindow::setData(DMAIndicator::getInstance().buffer->buffer);
}

DMAIndicator::DMAIndicator() {
	buffer = nullptr;
}

void DMAIndicator::DMAstart(uint32_t BufferSize) {
	sendMessage.resize(BufferSize);
	MainWindow::TimeStart(reinterpret_cast<TIMERPROC>(MyTimerProc));
}

DMAIndicator& DMAIndicator::getInstance() {
	static DMAIndicator dma;
	return  dma;
}

void DMAIndicator::setMemoryBaseAddr(Buffer& buf) {
	buffer = &buf;
	//BaseAddr = reinterpret_cast<uint32_t*>(addr);
}

void DMAIndicator::bringOutValue() {}
