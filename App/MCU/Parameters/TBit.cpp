#include "TBit.h"
#include "InternalAdresation.h"
#include "Slots/SlotHandlerType.h"
#include "bastypes.h"
#include "ini/parser.h"
#include "ParametersUtils.h"

#include <array>

const std::string TBit::SignalType = "TBit";
const std::array<u16, 16> TBit::Mask2BitNumber = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };

const std::string TBit::value(const TSlotHandlerArsg& args) {
    bool input = getRawValue(args);
    return (input) ? "1" : "0";
}

bool TBit::getRawValue(const TSlotHandlerArsg& args) {
    s16 offset = Addr.Addr - args.StartAddrOffset;
    u8* p = args.InputBuf + offset;//получил указатель на данные
    bauint raw;//получил два байта данных
    raw.b[0] = (*p++);
    raw.b[1] = (*p);
    bool res = static_cast<bool>(raw.i & Addr.Option);
    return res;
}

const std::string TBit::validation(const TSlotHandlerArsg& args) {
    if (args.InputBufValidBytes == 0) return "*";
    if (ParametersUtils::isAddrInvalid(Addr.Addr)) return "err.addr";
    if ((Addr.Addr < args.StartAddrOffset) || (Addr.Addr > args.LastAddrOffset)) return "out.addr";
    return "";
}

TBit::TBit(ISignal::PropsPointers props) : Parameter(props) {
    strAddr = IniParser::getInstance().getElementPtrByNumber(2, '/', props.pOptional);
    Addr = ParametersUtils::getSpecialAddrForBit(strAddr);
}

std::string TBit::getValue(const TSlotHandlerArsg& args, const char* format) {
    std::string res = validation(args);
    return (res != "")
        ? res
        : value(args);
}

const std::string TBit::getValueHex(std::string& src) {
    float f = std::stof(src);
    std::string res = (f != 0) ? "1" : "0";
    return res;
}

const std::string TBit::getRegHexAddr() {
    std::string res(strAddr + 1, 6);
    return res;
}

const std::string TBit::getWriteCmdType() {
    return "16";
}

const std::string& TBit::getSignalType() {
    return SignalType;
}

InternalMemAddress TBit::getInternalMemAddr() {
    s16 offset = Addr.Addr;
    s16 bitNumber = -1;
    u16 index = 0;
    for (auto& e : Mask2BitNumber) {
        if (e == Addr.Option) {
            bitNumber = index;
            break;
        }
        index++;
    }
    offset += (bitNumber < 7) ? 0 : 1;
    bitNumber -= (bitNumber < 7) ? 0 : 8;
    return { offset, 1, bitNumber };
}
