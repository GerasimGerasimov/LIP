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
