#include "com_master_driver.h"
//#include "ModbusMasterConf.h"
#include "ramdata.h"


//std::function<void()>
//TODO 
TDriverComReadEndHandler ComMasterDriver::onReadEnd = nullptr;
u8* ComMasterDriver::outbuf = nullptr;
u16 ComMasterDriver::OutBufLen = 0;
u16 ComMasterDriver::TimeOut = 0;
u8 ComMasterDriver::reply[256];
MBmasterSlotType ComMasterDriver::SlotMaster = MBmasterSlotType();
ModbusMasterConf ComMasterDriver::ModbusMaster = ModbusMasterConf(&SlotMaster);

static const s16 ERR_TIME_OUT = -1;


/* ComMasterDriver &ComMasterDriver::getInstance(){
  static ComMasterDriver comMasterDriver;
  return comMasterDriver;
} */

void ComMasterDriver::onReadData(void){
  
  if (ComMasterDriver::onReadEnd) {
    ComMasterDriver::onReadEnd(SlotMaster.InBufLen, reply);
  }
}

void ComMasterDriver::onTimeOut(void){
  
  if (ComMasterDriver::onReadEnd) {
    ComMasterDriver::onReadEnd(ERR_TIME_OUT, reply);
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