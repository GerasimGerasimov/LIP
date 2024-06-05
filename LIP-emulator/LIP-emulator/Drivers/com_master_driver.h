#pragma once

#include "stm32f0xx.h"
#include "framework.h"

#include <functional>

using TDriverComReadEndHandler = std::function<void(s16 result, u8* reply)>;

struct TComMasterTask
{
    u8* pbuff;
    u16 len;
    u16 TimeOut;
    TDriverComReadEndHandler callback;
};

class ComMasterDriver
{
private:
    static HANDLE handleCom;
    static HANDLE handleThread;
    static DWORD comThreatId;
    static u8* outBuf;
    static u16 outBufLen;
    static u16 TimeOut;
    static u8 reply[256];
    static TDriverComReadEndHandler onReadEdnd;
    static void create_com_thread();
    static DWORD WINAPI com_thread(LPVOID lpParam);
public:
    static void open();
    static void close();
    static void send(TComMasterTask task);
};

