#include "Buffer.h"



Buffer::Buffer(){

    status = Status::EMPTY;

}

uint32_t Buffer::getAddrBuffer(){
    return reinterpret_cast<uint32_t>(buffer.data());
}

uint16_t Buffer::getSize(){
    return buffer.size();
}

Buffer::Status Buffer::getStatus(){
    return status;
}

void Buffer::setFillStatus(){
    status = Status::FILL;
}

void Buffer::setEmptyStatus(){
    status = Status::EMPTY;
}


void Buffer::addData(std::vector<uint8_t>& data, uint16_t start, uint16_t end){
    if(!buffer.empty()){
        int bufferSize = buffer.size();
        int k = 0;
        for(int i = bufferSize - end; i < bufferSize - start; ++i){
            buffer[i] = data[k];
            ++k;
        }
    }
}

//для добавления элементов контейнер должен иметь необходимый размер
void Buffer::setSizeBuffer(uint8_t size){
    buffer.resize(size);
}
