#pragma once

#include "stm32f10x.h"

class OSResources
{
private:
	OSResources();
	OSResources(const OSResources&) = delete;
	OSResources& operator=(const OSResources&) = delete;
	OSResources(const OSResources&&) = delete;
	OSResources& operator=(const OSResources&&) = delete;
	~OSResources();

	u8* res;
	void init();
public:
	static OSResources& getInstance();
	const u8* getRes();
};

