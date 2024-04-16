#include "IniResources.h"
#include "Parameters/signal.h"
#include "Parameters/Scale.h"

#include <map>

std::map<std::string, std::map<std::string, std::map<std::string, ISignal*>>> Sources;

IniResources::IniResources() {
	Sources = {};
}

IniResources& IniResources::getInstance() {
	static IniResources resources;
	return resources;
}

std::string IniResources::getScaleValueByKey(const std::string& key, const std::string& dev) {
	if (Sources.count(dev)) {
		std::map<std::string, std::map<std::string, ISignal*>> devmap = Sources.at(dev);
		if (devmap.count("vars")) {
			std::map<std::string, ISignal*> vars = devmap.at("vars");
			if (vars.count(key)) {
				Scale* s = static_cast<Scale*>(vars.at(key));
				return s->getValue();
			}
		}
	}
	return "1.0";
}
