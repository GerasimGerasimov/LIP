#pragma once

#include <vector>

	enum class Status{
		SENT,
		READ
	};

class Buffer
{
private:
	std::vector<uint8_t> buffer;
	Status status;
	std::vector<uint8_t>::reverse_iterator iterator;
public:
	Buffer(uint8_t size);
	uint32_t getAddrBuffer();
	uint16_t getSize();
	Status getStatus();
	void swapStatus();
	void addData(std::vector<uint8_t>& data);
};

