#include "IniString.h"
#include "SignalFactoty.h"

ISignal* IniString::getSignal(const std::string& dev, const std::string& section, char* source, int scrLen){
    ISignal::PropsPointers props = SignalFactoty::getInstance().getSignalProps(dev.c_str(), source, scrLen);
    ISignal* s = SignalFactoty::getInstance().getSignal(props);
    return s;
}
