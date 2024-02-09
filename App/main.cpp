/**
  ******************************************************************************
  * @file    intmash\projects\CHG.CTRL.CPU\firmware\CHG.CTRL.CPU
  * @author  Sledin A.S. (������ �.�.)
  * @version V0.0.2
  * @date    30/04/2013
  * @brief   Main program body.
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"//���������� STM
#include "bastypes.h"//�������� ���� ������, ���������� ����� �������� RAM_DATA � FLASH_DATA
#include "ramdata.h"//���������� ���������� � ��������� RAM_DATA
#include "flashdata.h"//���������� ��������� � ��������� FLASH_DATA
#include "crc16.h"//������ ����������� �����
#include "init.h"//������� �������������

#include "fram/fram.h"
#include "lip/tirist.h"
#include "lip/fails.h"
#include "modbus/uart1rs485.h"//����� �� 485 ����������, �� ��������� MODBUS1 (������)
#include "modbus/uart2rs485.h"//����� �� 485 ����������, �� ��������� MODBUS2 (������)

#include "DEFINES.h" //��� ��������, ����������� ������ � ����� �������
#include "modbus/modbus.h"
#include "spi.h"
#include "Indicator/LIP_5Nx.h"
#include "Page/Page.h"

#include "stm32f10x_it.h"

    
/* defines ------------------------------------------------------------------*/
#define max_drebezg 0x0005//0x2000 // ������� ��� ����� ��������� ������� ������ ��� ���������� �������� ��������� 
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void START(void);
void STOP(void);
void Init_soft(void);
void Fail_Reset(void);
void Fail_Check(void);
/* Private variables ---------------------------------------------------------*/
    
    
/**
  * @brief  Main program.
  */
int main(void)              //������� ���������
{

  Init();                   //������������� ���������  


  
  LED_RUN_ON;
  LED_LINK1_OFF;
  LED_LINK2_OFF;
  LED_ALARM_OFF;
  /*�������������� ������������� �����, ������� ����� ��������� �������� ��������*/
  Init_soft();// ��� ����� ����� �  ��������� ��������
  Fail_Reset();//����� ������ ������
  //Page page;


  while (1)//�������� ���� ���������
  {    
    if ((U1_SwCNT()) ||(U2_SwCNT()))//������� ������ �� ������ �� ������� � 1 � 2 ����� ������� ��� ��������
    {
      if (LED_LINK1_ST) LED_LINK1_ON;
      else LED_LINK1_OFF; 
    }

    //page.update();
    
    
  }
}


//������ ������ ����-�����
void START(void)
{
  if (RAM_DATA.FLAGS.BA.TR_ENABLE == 0)//���� �� �� � ������ �����, ��� �� ��������� ������
  {
    //if (RAM_DATA.FLAGS.BA.DOUT1_FAIL == 0) //������ ���, ���� �� ���������
    //���� ���� ������ - ������ �� ������
     if (RAM_DATA.FLAGS.BA.SYNCF | RAM_DATA.FLAGS.BA.SHC | RAM_DATA.FLAGS.BA.OPC |\
         RAM_DATA.FLAGS.BA.OVH | RAM_DATA.FLAGS.BA.THFOC | RAM_DATA.FLAGS.BA.THFAO |\
          RAM_DATA.FLAGS.BA.FQS | RAM_DATA.FLAGS.BA.MTZ_L | RAM_DATA.FLAGS.BA.MTZ_SH)
    {    }
    else //���� ��� ������
    {
      Init_soft(); //����� ����� � ��������� ��������
      RAM_DATA.FLAGS.BA.TR_ENABLE =1; //��������� ��������� ���������� - ��������� ��� ������ ����   
    } 
  }
  else {} //�� ��������� ������� ����� �� ��������� � ������ ���������� ���
  
  RAM_DATA.FLAGS.BA.DI4_START = 0; //�������� ���� ������� ������, ����� �������� �� ���������������� ���
}
 

void STOP(void)
{
  if (RAM_DATA.FLAGS.BA.WAIT == 0)//��������� �� � ������ ��������, ������ ������� ����
  {
     RAM_DATA.FLAGS.BA.A_STOP = 1; // ���� ��������� ���� ����� ����������      ,
    // 
  }
  else //��� � ��������� ��������, ������ ������������
  {
    Fail_Reset();
//���� ������ ��������� �� ����������, ����� ����� ������ �� ����������� ���� ���� ���������
  }
  RAM_DATA.FLAGS.BA.DI3_STOP = 0; //�������� ���� ������� ������
   
 
}


/*������������� ������ � ���������� ���������������*/
void Init_soft(void)
{
    /*�������������� ������������� �����, ������� ����� ��������� �������� ��������*/
  //��� ����� ��� � ��� ��������
  RAM_DATA.FLAGS.BA.DI1 = 0;
  RAM_DATA.FLAGS.BA.DI2_BURNING = 0;
  RAM_DATA.FLAGS.BA.DI3_STOP = 0;
  RAM_DATA.FLAGS.BA.DI4_START = 0;
  RAM_DATA.FLAGS.BA.DI5_TERM = 0;
  RAM_DATA.FLAGS.BA.DOUT1_FAIL = 0;
  RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 0;
  RAM_DATA.FLAGS.BA.DOUT3_SIGN = 0;
  
  RAM_DATA.FLAGS.BA.DOUT4_SHAKE2 = 0;
  RAM_DATA.FLAGS.BA.DOUT5 = 0;
  RAM_DATA.FLAGS.BA.DOUT6 = 0;
  RAM_DATA.FLAGS.BA.DI6 = 0;
  
  RAM_DATA.FLAGS.BA.A_STOP = 0; // ���� ��������� ���� ����� ����������
  RAM_DATA.FLAGS.BA.WAIT = 0;//����� ����� ��������� ��������
  RAM_DATA.FLAGS.BA.SPARK = 0;//����� ����� �����
  RAM_DATA.FLAGS.BA.TR_ENABLE =0; //��������� ��������� ���������� - ��������� ��� ������ ����
  RAM_DATA.FLAGS.BA.TR1 = 0;//����� ����� ����������
  RAM_DATA.FLAGS.BA.TR2 = 0;//����� ����� ����������
  
  RAM_DATA.A = FLASH_DATA.Amax; //���� ����� ������������
  RAM_DATA.Spark_cnt = 0; //�������� �������, ������� ������� �����
  
  //����� ��� ���� � �����?
  RAM_DATA.Iz = FLASH_DATA.Iz; 
  RAM_DATA.Uz = FLASH_DATA.Uz;
  RAM_DATA.Az = FLASH_DATA.Amax;
  
  /*��������� ����������, ������������ � ������ ���������� � �����������*/
  NormalMode = true; //��������� ���� ����������, ������ � ������ ����������
  Pause_Imp = 0;
 // SYNC_Yes = 0;               
                 
                 
}
//����� ���� ������
void Fail_Reset(void)
{
  RAM_DATA.FLAGS.BA.SYNCF = 0;
  RAM_DATA.FLAGS.BA.MTZ_SH = 0;
  RAM_DATA.FLAGS.BA.MTZ_L = 0;
  RAM_DATA.FLAGS.BA.SHC = 0;
  RAM_DATA.FLAGS.BA.OPC = 0;
  RAM_DATA.FLAGS.BA.OVH = 0;
  RAM_DATA.FLAGS.BA.THFOC = 0;
  RAM_DATA.FLAGS.BA.THFAO = 0;
  RAM_DATA.FLAGS.BA.FQS = 0;
  
  RAM_DATA.FLAGS.BA.DOUT1_FAIL = 0;
  RAM_DATA.FLAGS.BA.DOUT3_SIGN = 0; //������ ������ ������
}

//�������� �� ������� ������ � �������� ��� ������� �����-����
void Fail_Check(void)
{
  if (  RAM_DATA.FLAGS.BA.MTZ_L | RAM_DATA.FLAGS.BA.MTZ_SH ) //������ ���
  {
    RAM_DATA.FLAGS.BA.TR_ENABLE =0; //��� ���������
    RAM_DATA.FLAGS.BA.WAIT = 1; //����� �������� ���
    RAM_DATA.FLAGS.BA.DOUT1_FAIL = 1; //������ �������
    RAM_DATA.FLAGS.BA.DOUT3_SIGN = 1; //������ ������ ������
    RAM_DATA.A = FLASH_DATA.Amax; //���� ����� ��������� ������������
    return; //����� � �� ��������� ���������
  }
  if (RAM_DATA.FLAGS.BA.SYNCF)
  {
    RAM_DATA.FLAGS.BA.TR_ENABLE =0; //��� ���������
    RAM_DATA.FLAGS.BA.WAIT = 1; //����� �������� ���
    RAM_DATA.FLAGS.BA.DOUT3_SIGN = 1; //������ ������ ������
    RAM_DATA.A = FLASH_DATA.Amax; //���� ����� ��������� ������������
    return; //����� � �� ��������� ���������
  }
  //����� ������ ������ - �� ��� �������
  if ( RAM_DATA.FLAGS.BA.SHC | RAM_DATA.FLAGS.BA.OPC |\
  RAM_DATA.FLAGS.BA.OVH | RAM_DATA.FLAGS.BA.THFOC | RAM_DATA.FLAGS.BA.THFAO | RAM_DATA.FLAGS.BA.FQS)
  {
    RAM_DATA.FLAGS.BA.DOUT3_SIGN = 1; //������ ������ ������
    STOP(); //������� ��������� ����/��������
  }
  else {}
    
}

























#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
