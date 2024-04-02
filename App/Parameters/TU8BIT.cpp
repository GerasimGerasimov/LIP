#include "TU8BIT.h"
#include "InternalAdresation.h"
#include "Slots/SlotHandlerType.h"
#include "bastypes.h"
#include "ini/parser.h"
#include "Utils.h"
#include "ParametersUtils.h"
#include "scaleutils.h"

const std::string TU8BIT::SignalType = "TU8BIT";

const std::string TU8BIT::value(const TSlotHandlerArsg& args, const char* format) {
    u8 input = getRawValue(args);
    float res = input * Scale;
    return Utils::getValueAsFormatStr(res, Utils::getFormat(res));
}

u8 TU8BIT::getRawValue(const TSlotHandlerArsg& args) {
    s16 offset = Addr.Addr - args.StartAddrOffset;
    u8* p = args.InputBuf + offset;//получил указатель на данные
    bauint raw;//получил два байта данных
    raw.b[0] = (*p++);
    raw.b[1] = (*p);
    u8 res = (Addr.Option == 1) ? raw.b[1] : raw.b[0];
    return res;
}

const std::string TU8BIT::validation(const TSlotHandlerArsg& args) {
    if (args.InputBufValidBytes == 0) return "**.*";
    if (ParametersUtils::isAddrInvalid(Addr.Addr)) return "err.addr";
    if ((Addr.Addr < args.StartAddrOffset) || (Addr.Addr > args.LastAddrOffset)) return "out.addr";
    return "";
}

u16 TU8BIT::string2raw(std::string& src) {
    float f = std::stof(src);
    f /= Scale;
    return static_cast<u16>(f);
}

TU8BIT::TU8BIT(ISignal::PropsPointers props) : Parameter(props)
    , MSU(IniParser::getInstance().getElementPtrByNumber(2, '/', props.pOptional)){
    strAddr = IniParser::getInstance().getElementPtrByNumber(1, '/', props.pOptional);
    Addr = ParametersUtils::getSpecialAddrForByte(strAddr);
    Scale = ScaleUtils::getScaleFromProps(props.dev, props.pOptional);
}

std::string TU8BIT::getMSU() {
    return (MSU)
		? IniParser::getInstance().getElement('/', MSU)
		: "";
}

std::string TU8BIT::getValue(const TSlotHandlerArsg& args, const char* format) {
    std::string res = validation(args);
	return (res != "")
		? res
		: value(args, format);
}

const std::string TU8BIT::getValueHex(std::string& src) {
    u16 value = string2raw(src);
    char s[8];
    sprintf(s, "%.2X", value);
    std::string res(s);
    return res;
}

const std::string TU8BIT::getRegHexAddr() {
    std::string res(strAddr + 1, 6);
    return res;
}

const std::string TU8BIT::getWriteCmdType() {
    return "16";
}

const std::string& TU8BIT::getSignalType() {
    return SignalType;
}

InternalMemAddress TU8BIT::getInternalMemAddr() {
    s16 offset = Addr.Addr;
    offset += Addr.Option;//0-L, 1-H byte
    return { offset,1,-1 };
}
