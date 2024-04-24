#pragma once

#include <string>

namespace IniResources
{
	std::string getScaleValueByKey(const std::string& key, const std::string& dev);
	std::string getSection(std::string section);
};

