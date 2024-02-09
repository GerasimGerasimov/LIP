#include "Buffer.h"

Buffer::Buffer(uint8_t size) {
	buffer.resize(size);
	status = Status::READ;
	iterator = buffer.rbegin();
}

uint32_t Buffer::getAddrBuffer() {
	return reinterpret_cast<uint32_t>(buffer.data());
}

uint16_t Buffer::getSize() {
	return buffer.size();
}

Status Buffer::getStatus() {
	return status;
}

void Buffer::swapStatus() {
	if (status == Status::READ) {
		status = Status::SENT;
	}
	else {
		status = Status::READ;
	}
	iterator = buffer.rbegin();
}

void Buffer::addData(std::vector<uint8_t>& data) {
	for (const auto& i : data) {
		*iterator = i;
		++iterator;
	}
}
