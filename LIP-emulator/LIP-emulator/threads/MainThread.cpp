#include "MainThread.h"
#include "framework.h"

namespace MainThread {
	HANDLE hMCUMainThread = NULL;
	DWORD  dwMCUMainThreadId = 0;
}

DWORD WINAPI mcuMain(LPVOID lpParam) {

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
