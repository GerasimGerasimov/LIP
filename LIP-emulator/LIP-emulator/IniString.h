#pragma once

#include <vector>
#include <string>

class ISignal;

namespace IniString {
	ISignal* getSignal(const std::string& dev, const std::string& section, char* source, int scrLen);
}