#pragma once

#include "stm32f0xx.h"

#include <vector>
#include <string>

struct ItemLimits {
	char* RootOffset;
	int Size;
};

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
	bool isRequiredName(char* Name1, const char* Name2);
	Item* getItemByName(const char* Name);
public:
	static InternalResources& getInstance();
	std::string getItemStringByName(const char* Name);
	ItemLimits getItemLimitsByName(const char* Name);
};

