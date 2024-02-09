#include "fails.h"
#include "ramdata.h"
#include "flashdata.h"
#include "stm32f10x.h"

#include "DEFINES.h" //��� ��������, ����������� ������ � ����� �������


//������� �������� �� ������, � �������� �� ����� �� ��������,
//� ����� ��������� � ������ ������, �� ����� �� ���������� - ����� ������ �������

/*1 ������ - mtz �� �����*/
void Ish_mtz_fail(void)
{
  if (RAM_DATA.Ish_avg > FLASH_DATA.Ish_mtz) RAM_DATA.FLAGS.BA.MTZ_SH = 1; //��������� ���� ������
}

/*2 ������ - mtz �� ��������*/
void Iload_mtz_fail(void)
{
  //if (RAM_DATA.Iload_avg > FLASH_DATA.Iload_mtz) RAM_DATA.FLAGS.BA.MTZ_L = 1; //��������� ���� ������
}

/*3 ������ - ��������*/
void OVH_fail(void)
{
  if (RAM_DATA.FLAGS.BA.DI5_TERM) RAM_DATA.FLAGS.BA.OVH = 1; //��������� ���� ������
}

/*4 ������ - ��*/
void SC_fail(void)
{
  //if (RAM_DATA.Uload_avg < FLASH_DATA.Uload_SHC) 
  //{
  //  if (RAM_DATA.Iload_avg > FLASH_DATA.Iload_SHC)  RAM_DATA.FLAGS.BA.SHC = 1; //��������� ���� ������
 // }
}

/*5 ������ - ��*/
void OC_fail(void)
{
  //if (RAM_DATA.Uload_avg > FLASH_DATA.Uload_OPC) 
  //{
   // if (RAM_DATA.Iload_avg < FLASH_DATA.Iload_OPC)  RAM_DATA.FLAGS.BA.OPC = 1; //��������� ���� ������
 // }
}

u8 Spark_cnt_Buffer[10] = {0,0,0,0,0, 0,0,0,0,0};

/*7 ������ - ������ ������*/
void SPARK_fail(u8 CNT)
{
  u16 E_cnt = 0; //����� ����� ������
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
  
  if (E_cnt >= FLASH_DATA.Spark_fq)  RAM_DATA.FLAGS.BA.FQS = 1; //��������� ���� ������
  
}









