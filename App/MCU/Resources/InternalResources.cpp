#include "InternalResources.h"
#include "Resources/OSResources.h"
#include "crc16.h"


InternalResources::InternalResources() {
	init();
}

bool InternalResources::init() {
	Root = reinterpret_cast<Resources*>(OSResources::getInstance().getRes());
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

bool InternalResources::isRequiredName(char* Name1, char* Name2) {
	return (bool)(strcmp(Name1, Name2) == 0);
}

InternalResources& InternalResources::getInstance() {
	static InternalResources resource;
	return resource;
}

InternalResources::Item* InternalResources::getItemByName(char* Name) {
	for (const auto & item : ValidItems) {
		if (isRequiredName(item->Name, Name)) {
			return item;
		}
	}
	return nullptr;
}

std::string InternalResources::getItemStringByName(char* Name) {
	std::string res = "";
	const Item* item = getItemByName(Name);
	if (item) {
		res.append(((char*)((char*)Root + item->BinaryDataAddr)), item->BinaryDataSize);
	}
	return res;
}
