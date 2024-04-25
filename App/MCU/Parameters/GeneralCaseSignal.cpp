#include "GeneralCaseSignal.h"
#include "ini/parser.h"
#include "Helpers/Utils.h"
#include "ParametersUtils.h"
#include "Slots/SlotHandlerType.h"
#include "scaleutils.h"

typedef float (*TFuncRawToFloat) (GeneralCaseSignal::RawReturn& input);

static inline float getFloatFromU(GeneralCaseSignal::RawReturn& input) {
	float res = (float)input.raw.i;
	return res;
}

static inline float getFloatFromS(GeneralCaseSignal::RawReturn& input) {
	float res = (float)input.raw.s;
	return res;
}

static inline float getFloatFromF(GeneralCaseSignal::RawReturn& input) {
	float res = input.raw.f;
	return res;
}

static const TFuncRawToFloat FuncRawToFloat[] = {
	getFloatFromU,
	getFloatFromS,
	getFloatFromF
};

static inline float RawToFloat(GeneralCaseSignal::RawReturn& input) {
	float res = FuncRawToFloat[(u8)input.type](input);
	return res;
}



std::string GeneralCaseSignal::value(const TSlotHandlerArsg& args, const char* format) {
	RawReturn input = getRawValue(args);
	float res = RawToFloat(input) * Scale;
	return Utils::getValueAsFormatStr(res, Utils::getFormat(res));
}

GeneralCaseSignal::RawReturn GeneralCaseSignal::getRawValue(const TSlotHandlerArsg& args) {
    return { GeneralCaseSignal::ReturnType::U, 0 };
}

std::string GeneralCaseSignal::validation(const TSlotHandlerArsg& args) {
	if (args.InputBufValidBytes == 0) return "***.**";
	if (ParametersUtils::isAddrInvalid(Addr)) return "err.addr";
	if ((Addr < args.StartAddrOffset) || (Addr > args.LastAddrOffset)) return "out.addr";
	return "";
}

GeneralCaseSignal::GeneralCaseSignal(ISignal::PropsPointers props) : Parameter(props)
	, MSU(props.pOptional.MSU) {
	strAddr = props.pOptional.strAddr;
	Addr = ParametersUtils::getByteOffsetFromSlahedAddrStr(strAddr.c_str());
	Scale = props.pOptional.Scale;
}

std::string GeneralCaseSignal::getMSU() {
    return MSU;
}

std::string GeneralCaseSignal::getValue(const TSlotHandlerArsg& args, const char* format) {
    std::string res = validation(args);
    return (res != "") ? res : value(args, format);
}

const std::string GeneralCaseSignal::getRegHexAddr() {
    std::string res(strAddr.substr(1, -1));
    return res;
}

const std::string GeneralCaseSignal::getWriteCmdType() {
    return "10";
}
