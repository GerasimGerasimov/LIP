#include "fails.h"
#include "ramdata.h"
#include "flashdata.h"
#include "stm32f10x.h"

#include "DEFINES.h" //все основные, относящиеся только к плате дефайны


//функции проверки на аварии, в принципе их можно не вызывать,
//а сразу проверять в нужных местах, но чтобы не запутаться - будут вызовы функций

/*1 авария - mtz по входу*/
void Ish_mtz_fail(void)
{
  if (RAM_DATA.Ish_avg > FLASH_DATA.Ish_mtz) RAM_DATA.FLAGS.BA.MTZ_SH = 1; //выставили флаг аварии
}

/*2 авария - mtz по нагрузке*/
void Iload_mtz_fail(void)
{
  //if (RAM_DATA.Iload_avg > FLASH_DATA.Iload_mtz) RAM_DATA.FLAGS.BA.MTZ_L = 1; //выставили флаг аварии
}

/*3 авария - перегрев*/
void OVH_fail(void)
{
  if (RAM_DATA.FLAGS.BA.DI5_TERM) RAM_DATA.FLAGS.BA.OVH = 1; //выставили флаг аварии
}

/*4 авария - кз*/
void SC_fail(void)
{
  //if (RAM_DATA.Uload_avg < FLASH_DATA.Uload_SHC) 
  //{
  //  if (RAM_DATA.Iload_avg > FLASH_DATA.Iload_SHC)  RAM_DATA.FLAGS.BA.SHC = 1; //выставили флаг аварии
 // }
}

/*5 авария - хх*/
void OC_fail(void)
{
  //if (RAM_DATA.Uload_avg > FLASH_DATA.Uload_OPC) 
  //{
   // if (RAM_DATA.Iload_avg < FLASH_DATA.Iload_OPC)  RAM_DATA.FLAGS.BA.OPC = 1; //выставили флаг аварии
 // }
}

u8 Spark_cnt_Buffer[10] = {0,0,0,0,0, 0,0,0,0,0};

/*7 авария - частые пробои*/
void SPARK_fail(u8 CNT)
{
  u16 E_cnt = 0; //сумма всего буфера
  Spark_cnt_Buffer[9] = Spark_cnt_Buffer[8];
  Spark_cnt_Buffer[8] = Spark_cnt_Buffer[7];
  Spark_cnt_Buffer[7] = Spark_cnt_Buffer[6];
  Spark_cnt_Buffer[6] = Spark_cnt_Buffer[5];
  Spark_cnt_Buffer[5] = Spark_cnt_Buffer[4];
  Spark_cnt_Buffer[4] = Spark_cnt_Buffer[3];
  Spark_cnt_Buffer[3] = Spark_cnt_Buffer[2];
  Spark_cnt_Buffer[2] = Spark_cnt_Buffer[1];
  Spark_cnt_Buffer[1] = Spark_cnt_Buffer[0];
  Spark_cnt_Buffer[0] = CNT;
  
  E_cnt = (Spark_cnt_Buffer[9] +Spark_cnt_Buffer[8] +Spark_cnt_Buffer[7]+
           Spark_cnt_Buffer[6] +Spark_cnt_Buffer[5] +Spark_cnt_Buffer[4]+
           Spark_cnt_Buffer[3] +Spark_cnt_Buffer[2] +Spark_cnt_Buffer[1]+
             Spark_cnt_Buffer[0]  );
  
  if (E_cnt >= FLASH_DATA.Spark_fq)  RAM_DATA.FLAGS.BA.FQS = 1; //выставили флаг аварии
  
}









