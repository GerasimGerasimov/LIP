#pragma once

#include "Resources/InternalResources.h"
#include "stm32f10x.h"

class OSResources {
  public:
    void init();
    const u8* getRes(void);
    static OSResources& getInstance();
private:
    u8* res;
    OSResources();
	  OSResources(const OSResources&) = delete;
	  OSResources& operator=(const OSResources&) = delete;
	  OSResources(const OSResources&&) = delete;
	  OSResources& operator=(const OSResources&&) = delete;
	  
};
