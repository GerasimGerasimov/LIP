#include "com_master_driver.h"

HANDLE ComMasterDriver::handleCom = NULL;
HANDLE ComMasterDriver::handleThread = NULL;
DWORD ComMasterDriver::comThreatId = 0;
TDriverComReadEndHandler ComMasterDriver::onReadEdnd = nullptr;
u8* ComMasterDriver::outBuf = nullptr;
u16 ComMasterDriver::outBufLen = 0;
u16 ComMasterDriver::TimeOut = 0;
u8 ComMasterDriver::reply[256];

void ComMasterDriver::create_com_thread(){
    DWORD dwThrdParam = 1;
    handleThread = CreateThread(NULL, 0, com_thread, &dwThrdParam, CREATE_SUSPENDED, &comThreatId);
}

DWORD __stdcall ComMasterDriver::com_thread(LPVOID lpParam){
    DWORD ButesToRead;
    DWORD Count;
    DWORD fSuccess;
    while(true){
        fSuccess = WriteFile(handleCom, outBuf, outBufLen, &Count, NULL);
        //Sleep(TimeOut);
        ButesToRead = 256;
        fSuccess = ReadFile(handleCom, &reply, ButesToRead, &Count, NULL);
        s16 result = (fSuccess > 0) ? Count : -1;
        if(onReadEdnd){
            onReadEdnd(result, reply);
        }
        SuspendThread(handleThread); //приостановить поток
    }
    return 0;
}

void ComMasterDriver::open(){
    LPCTSTR pcCommPort = TEXT("COM3");
    handleCom = CreateFile(pcCommPort, GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, 0);
    if(handleCom != INVALID_HANDLE_VALUE){
        DCB dcb;
        DWORD fSuccess = 0;
        COMMTIMEOUTS CommTimeouts;
        fSuccess = GetCommTimeouts(handleCom, &CommTimeouts);
        SecureZeroMemory(&dcb, sizeof(DCB));
        fSuccess = GetCommState(handleCom, &dcb);
        dcb.DCBlength = sizeof(DCB);
        dcb.BaudRate = CBR_115200;
        dcb.Parity = NOPARITY;
        dcb.ByteSize = 8;
        dcb.StopBits = ONESTOPBIT;
        fSuccess = SetCommState(handleCom, &dcb);

        CommTimeouts.ReadIntervalTimeout = 20;
        CommTimeouts.ReadTotalTimeoutMultiplier = 0;
        CommTimeouts.ReadTotalTimeoutConstant = 500;
        CommTimeouts.WriteTotalTimeoutMultiplier = 0;
        CommTimeouts.WriteTotalTimeoutConstant = 0;
        fSuccess = SetCommTimeouts(handleCom, &CommTimeouts);
    }
    create_com_thread();
}

void ComMasterDriver::close(){
    PurgeComm(handleCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
    CloseHandle(handleThread);
    CloseHandle(handleCom);
}

void ComMasterDriver::send(TComMasterTask task){
    onReadEdnd = task.callback;
    outBuf = task.pbuff;
    outBufLen = task.len;
    TimeOut = task.TimeOut;
    ResumeThread(handleThread); //возобновление потока
}
