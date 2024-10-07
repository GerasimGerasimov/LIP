#pragma once
//#include "bastypes.h"
//#include "IntmashLib\Intmash_bastypes.h"
//#include "IntmashLib\ModbusSlave.h"
#include "mbtypes.h"
#ifdef __cplusplus
extern "C" u16 BootLoader(TClient * Slave);
extern "C" void jumpToApplication(void);
extern "C" bool isBootLoaderMustBeStart(void);
extern "C" bool isApplicationReadyToStart(void);
extern "C" bool isApplicationToBe();
#else
extern u16 BootLoader(TClient* Slave);
extern void jumpToApplication(void);
extern bool isBootLoaderMustBeStart(void);
extern bool isApplicationReadyToStart(void);
extern bool isApplicationToBe();
#endif

u16 getPagesList(TClient* Slave);