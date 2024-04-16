#include "MainThread.h"
#include "framework.h"
#include "MainWindow.h"

#include <vector>

namespace MainThread {
	HANDLE hMCUMainThread = NULL;
	DWORD  dwMCUMainThreadId = 0;
}

DWORD WINAPI mcuMain(LPVOID lpParam) {
	Sleep(1000);
	int byteCount = 15;
	std::vector<uint8_t> vecData(byteCount);
	for (int i = 0; i < byteCount; ++i) {
		vecData[i] = i;
	}
	MainWindow::setData(vecData);
	Sleep(5000);
	for (int i = 0; i < byteCount; ++i) {
		vecData[i] = ~(1 << (i % 8));
	}
	MainWindow::setData(vecData);
	return 0;
}

void MainThread::createMCU() {
	DWORD dwThrdParam = 1;
	hMCUMainThread = CreateThread(NULL, 0, mcuMain, &dwThrdParam, 0 , &dwMCUMainThreadId);
}

void MainThread::closeMCU() {
	if (hMCUMainThread != NULL) {
		CloseHandle(hMCUMainThread);
	}
}
