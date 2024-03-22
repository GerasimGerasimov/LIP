#pragma once

#include "ModbusConf.h"
#include "STM32F10xUSART2.h"

struct MBmasterSlotType {
	void (*OnTimeOut)(void);//��������� ������� ���������� ���� ������ �� ��������
	void (*OnRecieve)(void);//��� ������ ����� ��������� �����-�� ������
	u8 InBufLen;//������� ������ �������
};

class ModbusMasterConf : public ModbusConf
{
private:
	STM32F10xUSART2 UsartMaster;
protected:
	u16 TimeOut = 0;
	u8* ReplyPtr = 0;
	MBmasterSlotType* Slot;
public:
	ModbusMasterConf(MBmasterSlotType* slot);
	void SetCondition(u16 timeOut, u8* replyPtr);
	void Send(u8* data, u8 len);
	void Reboot();
};

