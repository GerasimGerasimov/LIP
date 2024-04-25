#include "Parameter.h"
#include "InternalAdresation.h"

Parameter::Parameter(char* source, int scrLen) 
	: ISignal(source, scrLen)
	, Comment(nullptr)
	, strAddr(nullptr) {
}

Parameter::Parameter(ISignal::PropsPointers props)
	: ISignal(props)
	, Comment(props.pComment)
	, strAddr(nullptr) {
}

const std::string Parameter::getValueHex(std::string& src) {
	return "No Value";
}

const std::string Parameter::getRegHexAddr() {
	return "No Reg";
}

const std::string Parameter::getWriteCmdType() {
	return "No command";
}

InternalMemAddress Parameter::getInternalMemAddr() {
	return { -1,0,-1 };
}

u8 Parameter::getSizeByte() {
	return 0;
}
