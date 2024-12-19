#include "TS16BIT.h"
#include "InternalAdresation.h"
#include "Slots/SlotHandlerType.h"
#include "bastypes.h"

GeneralCaseSignal::RawReturn TS16BIT::getRawValue(const TSlotHandlerArsg& args){
    s16 offset = Addr - args.StartAddrOffset;
    u8* p = args.InputBuf + offset;//получил указатель на данные
    bauint raw;//получил два байта данных
    raw.b[0] = (*p++);
    raw.b[1] = (*p);
    GeneralCaseSignal::RawReturn res;
    res.type = GeneralCaseSignal::ReturnType::S;
    res.raw.s = raw.s;
    return res;
}

TS16BIT::TS16BIT(ISignal::PropsPointers props) : GeneralCaseSignal(props){}

InternalMemAddress TS16BIT::getInternalMemAddr(){
    s16 offset = Addr;
    return {offset,2,-1};
}

u8 TS16BIT::getSizeByte(){
    return 2;
}
