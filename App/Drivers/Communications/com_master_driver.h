#ifndef COM_MASTER_DRIVER_H
#define COM_MASTER_DRIVER_H

#include <string>
#include <functional>
#include "stm32f10x.h"
#include "modbus/ModbusMasterConf.h"

//#include "icommastert.h"
//result >= 0 длина ответа
//result = -1 ошибка
using TDriverComReadEndHandler = std::function<void(s16 result, u8* reply)>;

struct TComMasterTask {
	u8* pbuff;
	u16 len;
	u16 TimeOut;
	TDriverComReadEndHandler callback;
};

class ComMasterDriver {
public:

	static void send(TComMasterTask& task);
	static void interruptHandler();
	static void TIMHandler();
private:

	static u8 reply[256];
	static MBmasterSlotType SlotMaster;
	static TDriverComReadEndHandler onReadEnd;
	static ModbusMasterConf ModbusMaster;
	static u8* outbuf;
	static u16 OutBufLen;
	static u16 TimeOut;
	static void onReadData(void);
	static void onTimeOut(void);
};

#endif