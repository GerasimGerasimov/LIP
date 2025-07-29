#ifndef ID_H
#define ID_H
#include "stm32f0xx.h"

#ifdef __cplusplus
extern "C" {
#endif 

void IDinit(const char* idText);
u8 GetDeviceIDLength();
const char* getID();

#ifdef __cplusplus
}
#endif

#endif
