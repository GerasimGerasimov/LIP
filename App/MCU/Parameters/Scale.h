#pragma once

#include "signal.h"

#include <vector>

class Scale : public ISignal
{
protected:
	int	ValueSize;
	std::string getValueFromList(std::vector<std::string> values);
public:
	struct Props {
		std::string pKey = "";
		char* pValue = nullptr;
		int	ValueSize = 0;
	};

	Scale(char* source, int scrLen);
	Scale(Props props);
	virtual std::string getName() override;
	virtual std::string getValue() override;
};

