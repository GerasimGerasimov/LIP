#include "TFloat.h"
#include "InternalAdresation.h"
#include "Slots/SlotHandlerType.h"
#include "bastypes.h"
#include "Helpers/Utils.h"

std::string TFloat::value(const TSlotHandlerArsg& args, const char* format) {
    GeneralCaseSignal::RawReturn input = getRawValue(args);
    float res = input.raw.f * Scale;
    /*особый подход для TFloat в вычислении формата строки*/
    return Utils::getValueAsFormatStr(res, Utils::getFloatFormat(res));
}

GeneralCaseSignal::RawReturn TFloat::getRawValue(const TSlotHandlerArsg& args) {
	s16 offset = Addr - args.StartAddrOffset;
	u8* p = args.InputBuf + offset;//получил указатель на данные
	baulong  raw;//получил 4 байта данных
	raw.b[0] = (*p++);
	raw.b[1] = (*p++);
	raw.b[2] = (*p++);
	raw.b[3] = (*p);
	GeneralCaseSignal::RawReturn res;
	res.type = GeneralCaseSignal::ReturnType::F;
	res.raw.f = raw.f;
	return res;
}

u32 TFloat::string2raw(std::string& src) {
	float f = std::stof(src);
	f /= Scale;
	u32 i;
	memcpy(&i, &f, 4);
	return i;
}

TFloat::TFloat(ISignal::PropsPointers props) : GeneralCaseSignal(props) {}

InternalMemAddress TFloat::getInternalMemAddr() {
	s16 offset = Addr;
	return { offset,4,-1 };
}

const std::string TFloat::getValueHex(std::string& src) {
	u32 value = string2raw(src);
	char s[10];
	//GIST "%.4X" преобразование числа в hex с заданным кол-вом значащих нулей
	sprintf(s, "%.8X", value);
	std::string res(s);
	return res;
}

u8 TFloat::getSizeByte() {
	return 4;
}
