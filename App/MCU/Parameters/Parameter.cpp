#include "Parameter.h"
#include "InternalAdresation.h"

Parameter::Parameter(char* source, int scrLen) 
	: ISignal(source, scrLen)
	, Comment("")
	, strAddr("") {
}

Parameter::Parameter(ISignal::PropsPointers props)
	: ISignal(props)
	, Comment(props.pComment)
	, strAddr("") {
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
