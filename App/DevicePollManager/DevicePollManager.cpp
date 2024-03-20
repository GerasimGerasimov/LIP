#include "DevicePollManager.h"
#include "crc16.h"
#include "modbus/uart2rs485.h"
#include "ramdata.h"
//#include "consolelog.h"

//#include "U1RamUpdate.h"

//static U1RAMSlot SlotU1RAM;

//std::vector <Slot> DevicePollManager::Slots = {};
//Slot* DevicePollManager::slot = nullptr;
//u16 DevicePollManager::idx = 0;
//u16 DevicePollManager::Status = (u16)DevicePollManagerStatus::TOGGLE_SLOT;
//u8 DevicePollManager::Reply[256] = {};
//s16 DevicePollManager::ReplyResult = 0;

DevicePollManager::DevicePollManager(){
	slot = nullptr;
	idx = 0;
	Status = DevicePollManagerStatus::TOGGLE_SLOT;
	ReplyResult = 0;
}

DevicePollManager& DevicePollManager::getInstance() {
	static DevicePollManager poolManager;
	return poolManager;
}

void DevicePollManager::init(std::vector <Slot> slots) {
	//SlotU1RAM.init();
	Slots = slots;//{ SlotU1RAM };
}

void DevicePollManager::execute(void) {

	RAM_DATA.data[1] = (u16)Status;
	switch (Status)
	{
	case DevicePollManagerStatus::SEND_REQUEST:
		//ComMasterDriver::send({ (u8*)&slot->OutBuf, slot->cmdLen, slot->TimeOut, {checkRespond} });
		++RAM_DATA.counter[1];
		TxDMA1Ch7(slot->cmdLen, (u8*)&slot->OutBuf);
		Status = DevicePollManagerStatus::WAIT_RESPOND;
		break;
	case DevicePollManagerStatus::WAIT_RESPOND:
		++RAM_DATA.counter[3];
		break;
	case DevicePollManagerStatus::PARSE_RESPOND:
		slot->validation(ReplyResult, (u8*) &Reply);
 		Status = DevicePollManagerStatus::TOGGLE_SLOT;
		break;
	case DevicePollManagerStatus::TOGGLE_SLOT:
		slot = getNextSlot();
		Status = setActionBySlot();
		
		break;
	default:
		break;
	}
}

DevicePollManager::DevicePollManagerStatus DevicePollManager::setActionBySlot(void) {
	if (slot == NULL) return DevicePollManagerStatus::TOGGLE_SLOT;

	if (slot->Flags & (u16)Slot::StateFlags::SKIP_SLOT) {
		return DevicePollManagerStatus::TOGGLE_SLOT;
	}
	else {
		return (slot->isIntervalDone())
			? DevicePollManagerStatus::SEND_REQUEST
			: DevicePollManagerStatus::TOGGLE_SLOT;
	}
}

Slot* DevicePollManager::getNextSlot(void) {
	u16 size = Slots.size();//1 и больше если в списке что-то есть
	u16 tmp_idx = idx;
	if (size > 0) {
		idx = (++tmp_idx >= size)
			? 0
			: tmp_idx;
		Slot* s = &Slots[idx];
		return s;
	}
	else {
		return nullptr;
	}
}

void DevicePollManager::checkRespond(s16 result, u8* reply) {
	if (result > 0) {
		std::memcpy(Reply, reply, result);
	} 
	ReplyResult = result;
	Status = DevicePollManagerStatus::PARSE_RESPOND;
}

Slot* DevicePollManager::getSlotByDevPosAndSection(const std::string& device, const std::string& section) {
	for (auto& s : Slots) {
		if ((s.Device == device) && (s.Section == section)) {
			return &s;
		}
	}
	return nullptr;
}

Slot* DevicePollManager::CreateCustomSlot(std::string devname, std::string section) {
	
	Slot* res = new Slot(devname, section, 0, 0);
        res->Flags |= (u16)Slot::StateFlags::SKIP_SLOT;
	res->TimeOut = 2000;
	return res;
}

void DevicePollManager::addSlot(Slot *newSlot){
	Slots.push_back(*newSlot);
}
