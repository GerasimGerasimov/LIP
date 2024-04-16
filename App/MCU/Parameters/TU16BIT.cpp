#include "TU16BIT.h"
#include "InternalAdresation.h"
#include "Slots/SlotHandlerType.h"
#include "bastypes.h"

GeneralCaseSignal::RawReturn TU16BIT::getRawValue(const TSlotHandlerArsg& args) {
    s16 offset = Addr - args.StartAddrOffset;
    u8* p = args.InputBuf + offset;//получил указатель на данные
    bauint raw;//получил два байта данных
    raw.b[0] = (*p++);
    raw.b[1] = (*p);
    GeneralCaseSignal::RawReturn res;
    res.type = GeneralCaseSignal::ReturnType::U;
    res.raw.i = raw.i;
    return res;
}

u16 TU16BIT::string2raw(std::string& src) {
    float f = std::stof(src);
    f /= Scale;
    return static_cast<u16>(f);
}

TU16BIT::TU16BIT(ISignal::PropsPointers props) : GeneralCaseSignal(props) {}

const std::string TU16BIT::getValueHex(std::string& src) {
    u16 value = string2raw(src);
    char s[8];
    //GIST "%.4X" преобразование числа в hex с заданным кол-вом значащих нулей
    sprintf(s, "%.4X", value);
    std::string res(s);
    return res;
}

InternalMemAddress TU16BIT::getInternalMemAddr() {
    s16 offset = Addr;
    return { offset,2,-1 };
}
