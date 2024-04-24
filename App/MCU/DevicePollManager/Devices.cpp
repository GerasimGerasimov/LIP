#include "Devices.h"
#include "Resources/InternalResources.h"
#include "ini/parser.h"

Devices::Devices() {
	std::string item = InternalResources::getInstance().getItemStringByName("DEVICES");
	std::vector<std::string> devices = Parser::splitString("/", item);
	for (const auto& dev : devices) {
		item = InternalResources::getInstance().getItemStringByName(dev.c_str());
		if (item != "") {
			NetworkProps props = getNetworkProps(item);
			ListDevise[dev] = props;
		}
	}
}

Devices::NetworkProps Devices::getNetworkProps(std::string& dev) {
	NetworkProps props{"", "", 0};
	std::vector<std::string> currentDev = Parser::splitString("/", dev);
	props.Source = currentDev[0];
	props.COM = currentDev[1];
	props.NetworkAddr = std::stoi(currentDev[2]);
	return props;
}

Devices& Devices::getInstance() {
	static Devices device;
	return device;
}

u16 Devices::getDevNetWorkAddr(std::string position) {
	if (ListDevise.count(position)) {
		return ListDevise.at(position).NetworkAddr;
	}
	return 0;
}

std::string Devices::getSourceOfDev(std::string position) {
	if (ListDevise.count(position)) {
		return ListDevise.at(position).Source;
	}
	return "";
}
