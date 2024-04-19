#include "DevicePollManager.h"
#include "com_master_driver.h" //TODO
//#include "ramdata.h"

void checkRespond(s16 result, u8* reply) {
	if (result > 0) {
		std::memcpy(DevicePollManager::getInstance().Reply, reply, result);
	} 
	DevicePollManager::getInstance().ReplyResult = result;
	DevicePollManager::getInstance().status = DevicePollManager::Status::PARSE_RESPOND;
}

DevicePollManager::DevicePollManager(){
	slot = nullptr;
	idx = 0;
	status = Status::TOGGLE_SLOT;
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

	
	TComMasterTask task; //TODO com_master_driver
		//static u8 Lo = 0;
		//static u8 Hi = 0;
  		//static std::vector<u8> command = {0x01, 0x10, 0x00, 0x06, 0x00, 0x01, 0x02, 0x00, 0x00 };
	switch (status)
	{
	case Status::SEND_REQUEST:
		//++Lo;
		//if(Lo == 255){
		//	Lo = 0;
		//	++Hi;
		//}
		//command[7] = Hi;
		//command[8] = Lo;
		//slot->addcmd(command);
		task.pbuff = (u8*)slot->OutBuf.data();
		task.len = slot->cmdLen;
		task.TimeOut = slot->TimeOut;
		task.callback = checkRespond;
		ComMasterDriver::send(task);
		//++RAM_DATA.counter[0];
		
		status = Status::WAIT_RESPOND;
		break;
	case Status::WAIT_RESPOND:
		
		break;
	case Status::PARSE_RESPOND:
		slot->validation(ReplyResult, (u8*) &Reply);
 		status = Status::TOGGLE_SLOT;
		break;
	case Status::TOGGLE_SLOT:
		
		slot = getNextSlot();
		status = setActionBySlot();
		
		break;
	default:
		break;
	}
}

DevicePollManager::Status DevicePollManager::setActionBySlot(void) {
	if (slot == NULL) return Status::TOGGLE_SLOT;

	if (slot->Flags & (u16)Slot::StateFlags::SKIP_SLOT) {
		return Status::TOGGLE_SLOT;
	}
	else {
		return (slot->isIntervalDone())
			? Status::SEND_REQUEST
			: Status::TOGGLE_SLOT;
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

void DevicePollManager::CreateSlotsByStart(){}