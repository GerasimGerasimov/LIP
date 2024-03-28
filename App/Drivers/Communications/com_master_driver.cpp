#include "com_master_driver.h"
#include "DevicePollManager/DevicePollManager.h"

TDriverComReadEndHandler ComMasterDriver::onReadEnd = nullptr;
u8* ComMasterDriver::outbuf = nullptr;
u16 ComMasterDriver::OutBufLen = 0;
u16 ComMasterDriver::TimeOut = 0;
u8 ComMasterDriver::reply[256];
MBmasterSlotType ComMasterDriver::SlotMaster = MBmasterSlotType();
ModbusMasterConf ComMasterDriver::ModbusMaster = ModbusMasterConf(&SlotMaster);

static const s16 ERR_TIME_OUT = -1;

void ComMasterDriver::onReadData(void){
  
  if (ComMasterDriver::onReadEnd) {
    ComMasterDriver::onReadEnd(DevicePollManager::getInstance(), SlotMaster.InBufLen, reply);
  }
}

void ComMasterDriver::onTimeOut(void){
  
  if (ComMasterDriver::onReadEnd) {
    ComMasterDriver::onReadEnd(DevicePollManager::getInstance(), ERR_TIME_OUT, reply);
  }
}

void ComMasterDriver::send(TComMasterTask& task) {
    onReadEnd = task.callback;
    outbuf = task.pbuff;
    OutBufLen = task.len;
    TimeOut = task.TimeOut;
    SlotMaster.OnRecieve = &ComMasterDriver::onReadData;
    SlotMaster.OnTimeOut = &ComMasterDriver::onTimeOut;
    ModbusMaster.SetCondition(TimeOut, (u8*)&ComMasterDriver::reply);
    ModbusMaster.Send(outbuf, OutBufLen); 
}

void ComMasterDriver::interruptHandler(){
  ModbusMaster.interruptHandler();
}

void ComMasterDriver::TIMHandler(){
  ModbusMaster.TIMHandler();
}