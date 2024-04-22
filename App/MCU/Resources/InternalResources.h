#pragma once

#include "stm32f10x.h"

#include <vector>
#include <string>

class InternalResources
{
	struct Item {
		u32 BinaryDataAddr;
		u32 BinaryDataSize;
		char Name[14];
		u16 crc;
	};
	struct Resources {
		u32 TotalResourceSize;
		u16 NumberOfItems;
		u16 HeaderCRC;
		Item Items[];
	};
private:
	InternalResources();
	InternalResources(const InternalResources&) = delete;
	InternalResources& operator=(const InternalResources&) = delete;
	InternalResources(const InternalResources&&) = delete;
	InternalResources& operator=(const InternalResources&&) = delete;

	Resources* Root;
	std::vector<Item*> ValidItems;
	bool init();
	bool isHeaderCrcValid();
	bool isTotalCrcValid();
	bool isRequiredName(char* Name1, char* Name2);
	Item* getItemByName(char* Name);
public:
	static InternalResources& getInstance();
	std::string getItemStringByName(char* Name);
};

