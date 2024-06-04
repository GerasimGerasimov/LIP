#include "App.h"
#include "Page/Page.h"
#include "DevicePollManager/Slot.h"
#include "DevicePollManager/DevicePollManager.h"
#include "modbus/modbus.h"
#include "Router/Router.h"
#include "DevicePollManager/Devices.h"
//#include "OutStream.h"
//#include "ramdata.h"

#include <string>

App::App() {
}

App& App::getInstance() {
	static App app;
	return app;
}

void App::run() {

	//Page page;

	//Slot* slot = new Slot;
	//std::vector<u8> command = {0x01, 0x10, 0x00, 0x06, 0x00, 0x01, 0x02, 0x00, 0x55 };
	//std::vector<u8> command = { 0x01, 0x03, 0x00, 0x05, 0x00, 0x01 };

	//slot->addcmd(command);

	//slot->TimeOut = 100;
	//slot->onData = HandlerSlotRead::parseSlotRead;

	//DevicePollManager::getInstance().addSlot(slot);
	//InternalResources::getInstance();
	
	while (true) {
		processModbusSlave();
		Router::getInstance().update();
		DevicePollManager::getInstance().execute();
	}
}
