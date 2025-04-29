#include "DMAIndicator.h"
#include "Page/Page.h"
#include "Router/Router.h"
#include "MainWindow.h"
#include "Message/Message.h"

VOID CALLBACK MyTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime){
    if(Router::getInstance().isFillBuffer()){
        DMAIndicator::getInstance().DMAstart(Router::getInstance().getBufferSize());
    }
    static int delay = 0;
    if(delay == 10){
        delay = 0;
        LipMessage::getInstance().send_message(Event::TIMER, 0, 0);
    }
    ++delay;
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
