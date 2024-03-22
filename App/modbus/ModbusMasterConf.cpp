#include "ModbusMasterConf.h"

ModbusMasterConf::ModbusMasterConf(MBmasterSlotType* slot) : Slot(slot), UsartMaster(&UART, slot){
    Slot->OnTimeOut = 0;
    Slot->OnRecieve = 0;
}

void ModbusMasterConf::SetCondition(u16 timeOut, u8* replyPtr){
    UsartMaster.setCondition(timeOut, replyPtr);
}

void ModbusMasterConf::Send(u8* data, u8 len) {
    Slot->InBufLen = 0;
    UsartTransmit(&UART, data, len);
}

void ModbusMasterConf::Reboot() {
    UsartTxRxFinish(&UART);
}
