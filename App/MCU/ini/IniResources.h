#pragma once

#include <string>

class ISignal;

class IniResources
{
private:
	IniResources();
	IniResources(const IniResources&) = delete;
	IniResources& operator=(const IniResources&) = delete;
	IniResources(const IniResources&&) = delete;
	IniResources& operator=(const IniResources&&) = delete;

	
public:
	static IniResources& getInstance();
	std::string getScaleValueByKey(const std::string& key, const std::string& dev);
};

