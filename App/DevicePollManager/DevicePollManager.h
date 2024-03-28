#ifndef DEVICE_POLL_MANAGER_H
#define DEVICE_POLL_MANAGER_H

#include "stm32f10x.h"
//#include "com_master_driver.h"
#include "Slot.h"
#include <vector>

#define MB_MASTER_BUF_SIZE  256
#define MB_MASTER_SLOTS_MAX 24

//класс содержит слоты и управляет ими
class DevicePollManager {
enum class DevicePollManagerStatus {
	WAIT_IDLE = 1,
	WAIT_RESPOND = 2,
	PARSE_RESPOND = 3,
	TOGGLE_SLOT = 4,
	SEND_REQUEST = 5
};

public:
	static DevicePollManager& getInstance();

	void init(std::vector <Slot> slots);
	void execute(void);
	std::vector <Slot> Slots;
	DevicePollManagerStatus Status;
	Slot* getSlotByDevPosAndSection(const std::string& device, const std::string& section);
	Slot* CreateCustomSlot(std::string devname, std::string section);
	void addSlot(Slot* newSlot);
	void CreateSlotsByStart();
private:
	DevicePollManager();
	DevicePollManager(const DevicePollManager&) = delete;
	DevicePollManager& operator=(const DevicePollManager&) = delete;
	DevicePollManager(const DevicePollManager&&) = delete;
	DevicePollManager& operator=(const DevicePollManager&&) = delete;

	void checkRespond(s16 result, u8* reply);
	Slot* slot;
	u16 idx;
	Slot* getNextSlot(void);
	DevicePollManagerStatus setActionBySlot(void);
	u8 Reply[256];
	s16 ReplyResult;

};

#endif