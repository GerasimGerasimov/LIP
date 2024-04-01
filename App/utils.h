#pragma once

#include <string>
#include "stm32f10x.h"
//#include "bastypes.h"

namespace Utils {

	void swp_copy_u16(u8* s, u16* d, u8 c);
	void comma_to_dot(char* input);
	std::string getValueAsFormatStr(float& value, const char* format);
	const std::string UnSignedintToStr(u32 value);
	const char*& getFormat(float& value);
	const char*& getFloatFormat(float& value);
};

