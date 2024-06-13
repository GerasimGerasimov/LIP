#include "Slot.h"
#include "crc16.h"

Slot::Slot()
    : Flags(0)
    , RespondLenghtOrErrorCode(0)
    , onData(nullptr)
    , cmdLen(0)
    , StartAddrOffset(0)
    , LastAddrOffset(0)
    , Interval(0)
    , TmpInterval(0)
    , InputBufValidBytes(0)
    , TimeOut(0){}

Slot::Slot(std::string device, std::string section, u16 StartAddr, u16 LastAddr)
    : Flags(0)
    , RespondLenghtOrErrorCode(0)
    , onData(nullptr)
    , cmdLen(0)
    , Device(device)
    , Section(section)
    , StartAddrOffset((StartAddr & 0x00FF) << 1)
    , LastAddrOffset((LastAddr & 0x00FF) << 1)
    , Interval(0)
    , TmpInterval(0)
    , InputBufValidBytes(0)
    , TimeOut(0){

}

void Slot::init(void){

}

//добавление команды массивом
void Slot::addcmd(u8 cmd[], u8 size){
    OutBuf.resize(size + 2);
    std::memcpy(OutBuf.data(), cmd, size);
    InputBuf.resize(OutBuf[5] * 2 + 5);
    cmdLen = size + 2;
    FrameEndCrc16((u8*)OutBuf.data(), cmdLen);
}

//добавление команды std::vector
void Slot::addcmd(const std::vector<u8>& v){
    //GIST копирование вектора в массив
    OutBuf.resize(v.size() + 2);
    std::memcpy(OutBuf.data(), v.data(), v.size());
    InputBuf.resize(OutBuf[5] * 2 + 5);
    cmdLen = v.size() + 2;
    FrameEndCrc16((u8*)OutBuf.data(), cmdLen);
}

bool Slot::isReplyCRCValid(s16 result, u8* reply){
    return (result <= 0)
        ? false
        : (bool)(crc16(reply, result) == 0);
}

//проверка CRC вызов обработчика прочитанных данных
void Slot::validation(s16 result, u8* reply){
    (isReplyCRCValid(result, reply))
        ? (resetFlag(StateFlags::CRC_ERR))
        : (setFlag(StateFlags::CRC_ERR));
    RespondLenghtOrErrorCode = result;
    if(onData)
        onData(this, reply);
}

bool Slot::isIntervalDone(){
    if(++TmpInterval > Interval){
        TmpInterval = 0;
        return true;
    }
    return false;
}

void Slot::setFlag(StateFlags newFlag){
    Flags |= static_cast<u16>(newFlag);
}

void Slot::resetFlag(StateFlags delFlag){
    Flags &= ~(static_cast<u16>(delFlag));
}

bool Slot::isStateFlag(StateFlags isFlag){
    return (Flags & (static_cast<u16>(isFlag)));
}

Slot::~Slot(){}

