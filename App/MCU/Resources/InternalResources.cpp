#include "InternalResources.h"
#include "Drivers/Resources/os_resources.h"
#include "crc16.h"


InternalResources::InternalResources() {
	init();
}

bool InternalResources::init() {
	Root = (Resources*)OSResources::getInstance().getRes();
	if (!isHeaderCrcValid()) return false;
	if (!isTotalCrcValid()) return false;
	u16 i = 0;
	while (i < Root->NumberOfItems) {
		Item* p = &Root->Items[i++];
		if (crc16((u8*)p, sizeof(Item)) == 0) {
			ValidItems.push_back(p);
		}
		else {
			ValidItems.clear();
			return false;
		}
	}
	return true;
}

bool InternalResources::isHeaderCrcValid() {
	u8* p = reinterpret_cast<u8*>(&Root->TotalResourceSize);
	u16 crc = crc16(p, 8);//проверяю первые 8 байт заголовка
	return (crc == 0);
}

bool InternalResources::isTotalCrcValid() {
	u8* p = reinterpret_cast<u8*>(&Root->TotalResourceSize);
	u16 crc = crc16(p, Root->TotalResourceSize);
	
	return (crc == 0);
}

bool InternalResources::isRequiredName(char* Name1, const char* Name2) {
	return (bool)(strcmp(Name1, Name2) == 0);
}

InternalResources& InternalResources::getInstance() {
	static InternalResources resource;
	return resource;
}

InternalResources::Item* InternalResources::getItemByName(const char* Name) {
	for (const auto & item : ValidItems) {
		if (isRequiredName(item->Name, Name)) {
			return item;
		}
	}
	return nullptr;
}

std::string InternalResources::getItemStringByName(const char* Name) {
	std::string res = "";
	ItemLimits itemLimit = getItemLimitsByName(Name);
	res.append(itemLimit.RootOffset, itemLimit.Size);
	return res;
}

ItemLimits InternalResources::getItemLimitsByName(const char* Name) {
	ItemLimits res = { 0,0 };
	const Item* item = getItemByName(Name);
	if (item) {
		res.Size = item->BinaryDataSize;
		res.RootOffset = (char*)((char*)Root + item->BinaryDataAddr);
	}
	return res;
}
