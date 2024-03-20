#ifndef COM_MASTER_DRIVER_H
#define COM_MASTER_DRIVER_H

#include <string>
#include <functional>
#include "stm32f10x.h"
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
	static ComMasterDriver& getInstance();

	void open();
	void send(TComMasterTask task);
	void close();
private:
	ComMasterDriver();
	ComMasterDriver(const ComMasterDriver&) = delete;
	ComMasterDriver& operator=(const ComMasterDriver&) = delete;
	ComMasterDriver(const ComMasterDriver&&) = delete;
	ComMasterDriver& operator=(const ComMasterDriver&&) = delete;

	void com_thread(void);
	void create_com_thread(void);
	static TDriverComReadEndHandler onReadEnd;
	u8* outbuf;
	u16 OutBufLen;
	u16 TimeOut;
	static u8 reply[256];
	static void onReadData(void);
	static void onTimeOut(void);
};

#endif