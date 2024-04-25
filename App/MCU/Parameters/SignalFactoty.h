#pragma once

#include "signal.h"
#include "Scale.h"

#include <functional>
#include <map>

class SignalFactoty
{
private:
	SignalFactoty();
	SignalFactoty(const SignalFactoty&) = delete;
	SignalFactoty& operator=(const SignalFactoty&) = delete;
	SignalFactoty(const SignalFactoty&&) = delete;
	SignalFactoty& operator=(const SignalFactoty&&) = delete;
	const static std::map<std::string, std::function<ISignal* (ISignal::PropsPointers)>> TypeToSignal;
public:
	static SignalFactoty& getInstance();

	ISignal::PropsPointers getSignalProps(const char* dev, const char* source, const int srcLen);
	Scale::Props getScaleProps(const char* source, const int srcLen);
	ISignal* getSignal(const ISignal::PropsPointers& props);
	ISignal* getScale(const Scale::Props& props);
};

