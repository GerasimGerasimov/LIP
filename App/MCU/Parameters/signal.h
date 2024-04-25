#pragma once

#include <string>

class TSlotHandlerArsg;

class ISignal {
public:
struct PropsPointers{
	struct Optional {
		std::string Offset = "";
		std::string strAddr = "";
		std::string MSU = "";
		float Scale = 1.0;
		char* opt = nullptr; //нужен для TPrmList
	};
	std::string dev = "";
	std::string pName = "";
	std::string pComment = "";
	std::string pType = "";
	Optional pOptional;
};

	ISignal(char* source, int scrLen);
	ISignal(PropsPointers props);
	virtual ~ISignal();
	virtual std::string getName();
	virtual std::string getValue(const TSlotHandlerArsg& args, const char* format);
    virtual std::string getValue(){return "";};
	virtual std::string getMSU();
	virtual const std::string& getSignalType();
	std::string getDefaultValue();
protected:
	std::string Name;
	PropsPointers::Optional optional;
};

//typedef ISignal* pSignal;
