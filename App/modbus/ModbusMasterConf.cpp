#include "ModbusMasterConf.h"

ModbusMasterConf::ModbusMasterConf(MBmasterSlotType* slot) : Slot(slot), UsartMaster(&UART){
    Slot->OnTimeOut = 0;
    Slot->OnRecieve = 0;
}

void ModbusMasterConf::SetCondition(u16 timeOut, u8* replyPtr){
    TimeOut = timeOut;
    ReplyPtr = replyPtr;
}

void ModbusMasterConf::Send(u8* data, u8 len) {
    Slot->InBufLen = 0;
    UsartTransmit(&UART, data, len);
}

void ModbusMasterConf::Reboot() {
    UsartTxRxFinish(&UART);
}

void ModbusMasterConf::interruptHandler(){
        
    u16 TransferStatus =  UsartTxRxFinish(&UART);
    if (TransferStatus == 0){
        UsartMaster.SetTimer(TimeOut);//установили таймер на ожидание таймаута
        UsartRecieve(&UART, ReplyPtr);
    }
    else{
        UsartMaster.StopTimer();
        
        if(Slot->OnRecieve){
            Slot->InBufLen = TransferStatus;
            Slot->OnRecieve();
        }
    }
}

void ModbusMasterConf::TIMHandler(){
    
  if(Slot->OnTimeOut){
    Slot->OnTimeOut();
  }
    UsartMaster.StopTimer();
}