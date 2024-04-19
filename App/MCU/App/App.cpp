#include "App.h"
#include "Page/Page.h"
#include "DevicePollManager/Slot.h"
#include "DevicePollManager/DevicePollManager.h"
#include "modbus/modbus.h"
#include "Slots/HandlerSlotRead.h"
//#include "ramdata.h"

App::App() {
}

App& App::getInstance() {
	static App app;
	return app;
}

void parseRespond(Slot* slot, u8* reply) {

	//u8 regs_count = reply[2] >> 1;
	//swp_copy_u16((u8*)&reply[3], (u16*)&slot->InputBuf,	regs_count);
		  //slot->InputBufValidBytes = regs_count;
	slot->Flags |= (u16)Slot::StateFlags::DATA_VALID;

	//RAM_DATA.data[2] = reply[0];
	//RAM_DATA.data[3] = reply[1];
	//RAM_DATA.data[4] = reply[2];
	//RAM_DATA.data[5] = reply[3];
	//RAM_DATA.data[6] = reply[4];
	//RAM_DATA.data[7] = slot->InputBufValidBytes;

}

void App::run() {

	Page page;

	Slot* slot = new Slot;
	//std::vector<u8> command = {0x01, 0x10, 0x00, 0x06, 0x00, 0x01, 0x02, 0x00, 0x55 };
	std::vector<u8> command = { 0x01, 0x03, 0x00, 0x05, 0x00, 0x01 };

	slot->addcmd(command);

	slot->TimeOut = 100;
	slot->onData = HandlerSlotRead::parseSlotRead;//parseRespond;

	DevicePollManager::getInstance().addSlot(slot);

	while (true) {
		processModbusSlave();

		DevicePollManager::getInstance().execute();
	}
}
