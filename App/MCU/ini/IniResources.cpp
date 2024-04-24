#include "IniResources.h"
#include "Parameters/signal.h"
#include "Parameters/Scale.h"

#include <map>

const static std::map<std::string, std::string> SectionNameMap = { {"vars","[vars]"}, {"RAM","[RAM]"}, {"FLASH","[FLASH]"},{"CD","[CD]"} };

std::string IniResources::getScaleValueByKey(const std::string& key, const std::string& dev) {
	//if (Sources.count(dev)) {
	//	std::map<std::string, std::map<std::string, ISignal*>> devmap = Sources.at(dev);
	//	if (devmap.count("vars")) {
	//		std::map<std::string, ISignal*> vars = devmap.at("vars");
	//		if (vars.count(key)) {
	//			Scale* s = static_cast<Scale*>(vars.at(key));
	//			return s->getValue();
	//		}
	//	}
	//}
	return "1.0";
}

std::string IniResources::getSection(std::string section) {
	std::string newSection = "";
	if (SectionNameMap.count(section)) {
		newSection = SectionNameMap.at(section);
	}
	return newSection;
}
