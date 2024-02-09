#include "stm32f10x.h"
#include "bastypes.h"

volatile struct TRAM_DATA RAM_DATA;
u32 Eains[13];
vu16 id1_temp[5];
vu16 id2_temp[5];
vu16 num1_temp;
vu16 num1_passive;
vu16 num2_temp;
vu16 num2_passive;
vu8 FLASH_change;
vu16 EI1_per_cell;
vu16 EI2_per_cell;
vu8 gr1_fan_on_cnt;
vu8 gr1_fan_off_cnt;
vu8 gr1_err_cnt;
vu8 gr2_fan_on_cnt;
vu8 gr2_fan_off_cnt;
vu8 gr2_err_cnt;

