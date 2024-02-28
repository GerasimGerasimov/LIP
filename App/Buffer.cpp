#include "Buffer.h"

Buffer::Buffer() {
	
	status = Status::EMPTY;
	iterator = buffer.rbegin();
}

Buffer& Buffer::operator=(Buffer& buf) {
	buffer = buf.buffer;
	buf.swapStatus();
	swapStatus();
	return *this;
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
	if (status != Status::EMPTY) {
		status = Status::EMPTY;
		iterator = buffer.rbegin();
	}
	else {
		status = Status::FILL;
	}
}


void Buffer::addData(std::vector<uint8_t>& data) {
	if (!buffer.empty()) {
		for (const auto& i : data) {
			*iterator = i;
			++iterator;
		}
	}
}

//для добавления элементов контейнер должен иметь необходимый размер
void Buffer::setSizeBuffer(uint8_t size) {
	buffer.resize(size);
}
