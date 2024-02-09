#ifndef RAMDATA_H
#define RAMDATA_H
#include "bastypes.h"

//параметры расположеные RAM
extern struct TRAM_DATA RAM_DATA;

extern u32 Eains[13];
extern vu16 num1_temp;
extern vu16 num1_passive;
extern vu16 num2_temp;
extern vu16 num2_passive;
extern vu16 EI1_per_cell;
extern vu16 EI2_per_cell;
extern vu16 id1_temp[5];
extern vu16 id2_temp[5];
extern vu8 gr1_fan_on_cnt;
extern vu8 gr1_fan_off_cnt;
extern vu8 gr1_err_cnt;
extern vu8 gr2_fan_on_cnt;
extern vu8 gr2_fan_off_cnt;
extern vu8 gr2_err_cnt;
extern vu8 FLASH_change;




#endif