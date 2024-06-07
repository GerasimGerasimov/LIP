#include "DMAIndicator.h"
#include "Page/Page.h"
#include "Router/Router.h"
#include "MainWindow.h"

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime){
    if(Router::getInstance().isFillBuffer()){
        DMAIndicator::getInstance().DMAstart(Router::getInstance().getBufferSize());
    }
}

DMAIndicator::DMAIndicator(){
    buffer = nullptr;
}

void DMAIndicator::DMAstart(uint32_t BufferSize){
    MainWindow::setData(buffer->buffer);
    Router::getInstance().setEmptyBufferStatus();
}

void DMAIndicator::DMAstop(){}

DMAIndicator& DMAIndicator::getInstance(){
    static DMAIndicator dma;
    return  dma;
}

void DMAIndicator::setMemoryBaseAddr(Buffer& buf){
    buffer = &buf;
    MainWindow::TimeStart(reinterpret_cast<TIMERPROC>(MyTimerProc));
}

void DMAIndicator::bringOutValue(){}
