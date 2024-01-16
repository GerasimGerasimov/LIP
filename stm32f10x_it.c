           /**
  ******************************************************************************
  * @file    Examples/GPIOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x.h"
#include "bastypes.h"
#include "modbus/uart1rs485.h"
#include "modbus/uart2rs485.h"//����� �� 485 ����������, �� ��������� MODBUS (������)
#include "crc16.h"
#include "ramdata.h"
#include "flashdata.h"

#include "DEFINES.h" //��� ��������, ����������� ������ � ����� �������
#include "lip/tirist.h"
#include "lip/fails.h"

/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _175grad 0xAA79


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u8 num_avg = 0;
static u32 E_Uload = 0;
static u32 E_Iload = 0;
static u32 E_Ish = 0;

static u8 num_avg_Efiltr = 0;
static u32 Efiltr_Uload = 0;
static u32 Efiltr_Iload = 0;


static u16 dA = 0; //�� ������� ����������� �����
static u16 A_NormMode = 0; //�������� ������������ ���� = RAM_DATA.A +dA
 u8 Pause_Imp = 0; //������� ��������� �������� ��������� ����� ��������
 bool NormalMode = TRUE; //���� ������ ����� � �������� ���� ������� ��� ����������� ���� - false

static u16 Shake_cnt = 0; //������ �������� ��������� �������������
static u32 Shake_stop_cnt = 0; //����� ������� �������������, ���� �� �� �������

static u8 SYNC_cnt = 0; //������ �������� ��������� �������������
 u16 SYNC_Yes =0; //����������� ������ �� ������ ������, ����������� �������

//��� �������� �� ������ �����
static bool SPARK = FALSE; //��� ����� - �����
static u8 SPARK_cnt = 0; //������� ���� - � ���������� ���� ����� ��� ���
static u8 SP_buf_cnt = 0; //������� ������/�������� SPARK_cnt � ����� � �������� �� ������
u8 Spark_Pause_Imp=0; //akfu ������� ������������ - ������� �� ������ �������� �� ��� 
  
static u8 FAIL_flgcnt = 0; //������� ������� ��������� ���������, ���� ������
u16 Il_buf[100]; 
u16 Ul_buf[100];
u8 i_buf_main=0; //������� ��� ������ �����
/* Private function prototypes -----------------------------------------------*/
// ������� ����� �������� ��������� DMA. ��������� ���������� �����
/*#define Uth 208
#define Ith 150 //* 10% �� ������������ ���� 
#define Vcc 2087//1200
#define R1 26
#define R2 100
#define R3 (220+150) //* ����� ���������� �������� �� 0 �� 150 
*/
/* Private functions ---------------------------------------------------------*/



/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


void SysTickHandler(void)
{
 
}

//������ ����������
void TIM1_CC_IRQHandler (void)
{
  if ((TIM1->SR & TIM_FLAG_CC1)&&(TIM1->DIER & TIM_IT_CC1)) TIM1_user_U1();
  if ((TIM1->SR & TIM_FLAG_CC2)&&(TIM1->DIER & TIM_IT_CC2)) TIM1_user_U2();    
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
*******************************************************************************/
//��� ������ ����������
void TIM2_IRQHandler(void)
{
   TIM2->SR = 0;
}

//���������� ����������� - ���� � ���
void TIM4_IRQHandler(void)
{
   
 //  if ((TIM4->SR & TIM_FLAG_CC1)&&(TIM4->DIER & TIM_IT_CC1)) 
  if (TIM4->SR & TIM_IT_CC1)
   {
     TIM4->CR1 &= ~TIM_CR1_CEN;//���������� ������
     TIM4->SR = 0;     
     TxThyristorON(); //���������� ����, �������� ��� - ������� ���������� ����������
     TIM4->CR1 |= TIM_CR1_CEN;//TIMER1_START;
     
   }
 //  if ((TIM4->SR & TIM_FLAG_CC2)&&(TIM4->DIER & TIM_IT_CC2)) 
  if (TIM4->SR & TIM_IT_CC2)
   { 
     TIM4->CR1 &= ~TIM_CR1_CEN;//���������� ������
     TIM4->SR = 0;
     AllThyristorsOFF(); //������� ��������, ���� ���    
    
   }
   
   
}


//��� ������ � ��� - ������ 0,1 ��
void DMA1_Channel1_IRQHandler(void)//���������� ����������, ����� ������� ��� ������ � ���, ��������� � �������� ������� ��������
{ 
  static u8 i_buf=0; //������� ��� ������ �����
  static u8 i_buf_100ms=0; //������� ��� ������ �����
  // �������������� ���������� ��� ������ ��������� �������
  static s32 Ec=0; // ����������, ��������� ����� ������������
  s32 Uc =Ec/1000;
  s32 ic=0;

  
//RAM_DATA.V_Ref = ADC1->JDR1; //����������� ��������

  SYNC_Yes++; //������ ������ ������
  /*���������� ���� � ���������� �������,�������� ������������ Vref*/
   if(RAM_DATA._Uload < FLASH_DATA.Uload_offs) RAM_DATA._Uload = FLASH_DATA.Uload_offs; //����������� �� ���� �����
   if(RAM_DATA._Iload < FLASH_DATA.Iload_offs) RAM_DATA._Iload = FLASH_DATA.Iload_offs; //����������� �� ���� �����
   if(RAM_DATA._Ish < FLASH_DATA.Ish_offs) RAM_DATA._Ish = FLASH_DATA.Ish_offs; //����������� �� ���� �����
   RAM_DATA.Uload = ((u32)(RAM_DATA._Uload - FLASH_DATA.Uload_offs)* FLASH_DATA.KADC_Uload) / RAM_DATA.V_Ref;
   RAM_DATA.Iload = ((u32)(RAM_DATA._Iload - FLASH_DATA.Iload_offs) * FLASH_DATA.KADC_Iload) / RAM_DATA.V_Ref;
   RAM_DATA.Ish = ((u32)(RAM_DATA._Ish - FLASH_DATA.Ish_offs)* FLASH_DATA.KADC_Ish) / RAM_DATA.V_Ref;
   
  /*�������� �� ����� - �������� ������ ����� ��� �������*/
   if (RAM_DATA.FLAGS.BA.DI2_BURNING != 1)
   {
     ic = ((s32)RAM_DATA.Uload - Uc - FLASH_DATA.Vcc)*1000/(FLASH_DATA.R1+FLASH_DATA.R3);
     if( ((s32)RAM_DATA.Uload - Uc - ic*FLASH_DATA.R1/1000)> (s32)FLASH_DATA.Uth)//
     {
        ic = ((s32)RAM_DATA.Uload - Uc - (s32)FLASH_DATA.Uth)*1000/FLASH_DATA.R1 - Uc*1000/FLASH_DATA.R2;
     }
     else
     {
       ic -= Uc*1000/FLASH_DATA.R2;
       //���� ��� ������ ������ 0...10%, ������ ������� �����
       if (RAM_DATA.Iload > FLASH_DATA.Ith) //������ �����!
       {
         RAM_DATA.FLAGS.BA.SPARK = 1; //���� ������� ����� ��� EXTI 
         RAM_DATA.Spark_cnt ++; //������� ����� ��� EXTI
         SPARK = TRUE; //���� ����� ��� �������� �� ������ �����
       }
     } 
     Ec+=ic/10;
   }
   /*��� ������ ������ � ������*/
   if (RAM_DATA.load_buf==0)
   {
      Il_buf[i_buf] = RAM_DATA.Iload;
      Ul_buf[i_buf] = RAM_DATA.Uload;
      i_buf++;
      if (i_buf==100)i_buf=0;
   }
  /*��� ���������� �� ����������*/
   E_Uload += RAM_DATA.Uload;
   E_Iload += RAM_DATA.Iload;
   E_Ish += RAM_DATA.Ish;
   num_avg ++;
   if (num_avg == 100) //������ 10��
   {
     /*��� ������ ������� � ��������*/
     if (i_buf_100ms == 10)
     { i_buf_100ms =0;
        i_buf_main++;
        if (i_buf_main ==100) i_buf_main=0;
     }
     else i_buf_100ms ++;
     /*���������� �� ����������*/  
      RAM_DATA.Uload_avg = ((u32)E_Uload/num_avg);
      RAM_DATA.Iload_avg = ((u32)E_Iload/num_avg);
      RAM_DATA.Ish_avg = ((u32)E_Ish/num_avg);
      E_Uload = 0; //�������� ���������� ������������ ���������� ���
      E_Iload = 0 ;
      E_Ish = 0;
      num_avg =0;
      
      /*��� ��������� ����������� �� �������*/
      if (num_avg_Efiltr==50)//
      {
        RAM_DATA.Iload_Efiltr = Efiltr_Iload/num_avg_Efiltr;
        RAM_DATA.Uload_Efiltr = Efiltr_Uload/num_avg_Efiltr;
        Efiltr_Iload =0;
        Efiltr_Uload =0;
        num_avg_Efiltr = 0;
      }
      else 
      {
        Efiltr_Iload += RAM_DATA.Iload_avg;
        Efiltr_Uload += RAM_DATA.Uload_avg;
        num_avg_Efiltr++;
      }
      

      
      /*�������� �� ������*/
     /* if (RAM_DATA.FLAGS.BA.SPARK == 0)//���� � ���� ��� �� ���� ����� - ��������� �� mtz 
      {
        Ish_mtz_fail(); // mtz �� �����
        Iload_mtz_fail(); //mtz �� ��������
      }*/

      /*��������� ����� ���������� ����� ������ � ����� ��� ��������� � �����*/
      if (NormalMode == TRUE)//���� ���������� ����� ������
      {
        if (RAM_DATA.FLAGS.BA.A_STOP == 0)//� ���� ��� ���������
        {
          if (RAM_DATA.FLAGS.BA.WAIT == 0)//� ���� ������� ��������, �� �����
          {
            SC_fail(); //��
            OC_fail(); //��
          }
        }
      }
      
      /*�������� �� ������ �����*/
      if (SPARK == TRUE)//������ 10 �c, ���� � ���� ����������� ���� ����� - ���� ���������� �������� ���������� � ������������ � ����������
      {
        SPARK = FALSE;
        SPARK_cnt++;
      }
      else //���� � ���� ����������� �� ���� ����� - ���� �� ��������������� � ����� ��������� mtz
      {
        /*�������� �� ������*/
        // if (RAM_DATA.FLAGS.BA.SPARK == 0)//� ������ ��� ����������� ����� ������ �����
        if (Spark_Pause_Imp == 0)//� ������ ��� ����������� ����� ������ �����
        {
          Ish_mtz_fail(); // mtz �� �����
          Iload_mtz_fail(); //mtz �� ��������
        }
      }
      
      if (SP_buf_cnt == 10)//������ 100��
      {
        SPARK_fail(SPARK_cnt); //�������� ������ �� ������ �����
        SPARK_cnt= 0;
        SP_buf_cnt = 0;
      }
      else SP_buf_cnt++;
     
      /* �������� �� ������ - ��� ������������� � ������� 100 ��*/
      SYNC_cnt++; //������� ������ ������ 1 ��� = 10 ��
      if (SYNC_cnt >= SYNC_fail_cnt) RAM_DATA.FLAGS.BA.SYNCF = 1; //��������� ���� ������ ������ 
      
     /*���������/���������� �������������*/
      if(RAM_DATA.FLAGS.BA.TR_ENABLE == 1) //���� �������� ���������� ������� �����
      {
        if (RAM_DATA.FLAGS.BA.DOUT2_SHAKE) //���� ������������� ���
        {  
          if (Shake_cnt) Shake_cnt--; 
          else //������������� ���������
          {
            Shake_stop_cnt = ((FLASH_DATA.ShPeriod - FLASH_DATA.ShWidth)); //�������� ������������� �� ���� ������
            RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 0;//����. �������������
          }
        }
        else //������������� ��������
        { 
           if (Shake_stop_cnt) Shake_stop_cnt--; 
          else //����� ������� ������������� �����������
          {
            Shake_cnt = (FLASH_DATA.ShWidth); //�������� ������������� �� ���� ������
            RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 1;//���. �������������
          }
        }   
      }
      else
      {
        if (RAM_DATA.FLAGS.BA.DOUT2_SHAKE) //���� ������������� ���
        {  
          if (Shake_cnt) Shake_cnt--; 
          else //������������� ���������
          {
            Shake_stop_cnt = 0; //�������� ������������� �� ���� ������
            RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 0;//����. �������������
          }
        }
        else{ //���� ���� - ��� ������ ��������
          Shake_cnt=0;
          Shake_stop_cnt=0;
        }
      }
      
      /*����������� ������� ���������� ���������� �� ������*/
      if(RAM_DATA.FLAGS.BA.DOUT1_FAIL ==1)
      {
        if(FAIL_flgcnt == 100)
        { 
          FAIL_flgcnt =0;
          RAM_DATA.FLAGS.BA.DOUT1_FAIL =0;
        }
        else {FAIL_flgcnt++;}
      }
      
      
   }//���������� ������ 10�� 

      
   DMA1->IFCR |= DMA_ISR_TCIF1; //���������� ���� ��������� ������  
}


/*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
    s16 error = 0; //��� �-����������
    u32 dA_P = 0; //������ �����, ��������� �� �-����������   
    u16 Tim_limit = 0;// ����������� ��� �������, ����� �� ������� �� ������� ����������� ������
    if (EXTI->PR & EXTI_Line11)  // ���� ������ ������
    { 
      if (SYNC_Yes <90) //���� ������ ������ ������, ������� ����� ������ 50��
      {
        EXTI->PR = EXTI_Line11; // ���������� ���� ����������
        return; //�������
      }
      SYNC_Yes = 0; //�������� ��� ����������� ��������
      SYNC_cnt=0;
      if(RAM_DATA.FLAGS.BA.TR_ENABLE == 1) //���� ��������� ��������� ����������� - ���������!!
      {
        /*��������� ������� � ���� �����*/
        TIM4->CR1 &= ~TIM_CR1_CEN;//���������� ������
        TIM4->SR = 0;
        TIM4->CNT = 0; //��������, ������ ������� � 0
        AllThyristorsOFF(); //��������� ��������� � ����� ������ */
        
        /*���� � ��� � ������� ���������� ���� �����*/
        if (RAM_DATA.FLAGS.BA.SPARK == 1) 
        {  
          RAM_DATA.load_buf = 1;   //�������, ��� � ������ ������ ������ �����       
          RAM_DATA.FLAGS.BA.SPARK=0;
          NormalMode = FALSE; 
          Pause_Imp = FLASH_DATA.SkipImpSpark; //������� ��������� ����� ����� �������� 
          dA = ((u32)RAM_DATA.Spark_cnt * FLASH_DATA.K_SensSpark); //�� ������� ����������� �����
          
          RAM_DATA.Spark_cnt = 0; //�������� ������� ������������ �����
          A_NormMode = RAM_DATA.A +dA; //�������� ������ ����, �� �������� �����������
          RAM_DATA.Spark_cnt_view = dA;//������� �������� ��� ��������
          //��������� ������ ������������
          if (A_NormMode > (FLASH_DATA.Amax)) A_NormMode = FLASH_DATA.Amax; //����� ��������� ����������� ����� �� ������� �� ������� ������� Amax
          if (A_NormMode < (FLASH_DATA.Amin)) A_NormMode = FLASH_DATA.Amin; //����� ��������� ����������� ����� �� ������� �� ������� ������� Amax
          Pause_Imp --; //���� ���� ������� ��� ����������, ��� ��� -1
          Spark_Pause_Imp=1;
        }//���� �����
        else
        {
          if (RAM_DATA.FLAGS.BA.A_STOP ==1) //���� ���� ���������� ������� - ����������� ���� �
          {
             RAM_DATA.A = RAM_DATA.A + FLASH_DATA.VA_rise; //����������� ���� �� ������������� �������� Amax
             if (RAM_DATA.A > FLASH_DATA.Amax ) 
             {
               RAM_DATA.A = FLASH_DATA.Amax ; // �������� � ������ ������������� ���� Amax ?���� ��?
               RAM_DATA.FLAGS.BA.A_STOP = 0; // �������� ���� ��������� ���� ����� ����������
               RAM_DATA.FLAGS.BA.WAIT = 1;//����������  ����� ��������� ��������
               RAM_DATA.FLAGS.BA.TR_ENABLE =0; //��������� ��������� ���������� - ��������� ��� ������ ����
               EXTI->PR = EXTI_Line11; // ���������� ���� ����������
               return; //�������
             }
          }
          else //���������� ����� ������
          {
             /*������������ � ���������� ����*/
             if (NormalMode==FALSE) //���� ��� ������������ ���� ������ A_NormMode, �������� ����� ���� �������
             { 
               if (Pause_Imp > 0 )//���� �������� �� ��������� 
               { 
                  Pause_Imp --; //��������� �������
                  if (Pause_Imp==0) RAM_DATA.A = FLASH_DATA.Amax ; // �������� � ������ ������������� ���� Amax
                  EXTI->PR = EXTI_Line11; // ���������� ���� ����������
                  return; //�������
               }  
               Spark_Pause_Imp=0; //������ ��� ������� ��������
               //�������� ���������               
              if ((u32)RAM_DATA.A < ((u32)FLASH_DATA.VA_fall_spark+ (u32)A_NormMode))
              { RAM_DATA.A = A_NormMode;// //����� ����� �� ������� �� ������� A_NormMode
                if (RAM_DATA.A <(A_NormMode+1)) NormalMode = TRUE; //���� ����� �� ������������� �������� - ���������� ���������� �����, ���������� �������� �����
              }
              else  RAM_DATA.A = RAM_DATA.A - FLASH_DATA.VA_fall_spark; //������ ������� ���� �� A_NormMode 
             
             }  
             else  {RAM_DATA.A = RAM_DATA.A - FLASH_DATA.VA_fall_norm;} //���������� ����� - ������� � ���������� ���������
             if (RAM_DATA.A < FLASH_DATA.Amin ) RAM_DATA.A = FLASH_DATA.Amin;// //����� ����� �� ������� �� ������ ������� Amin
           }   
           /*�-���������*/
           error = (s32)RAM_DATA.Iload_avg - (s32)RAM_DATA.Iz; //FLASH_DATA.Iz;
           if (error > 0) dA_P = error * FLASH_DATA.Ky_I;  
           error = (s32)RAM_DATA.Uload_avg - (s32)RAM_DATA.Uz; //FLASH_DATA.Uz;
           if (error > 0) dA_P += (error * FLASH_DATA.Ky_U);
          // RAM_DATA.A = RAM_DATA.A + dA_P;
           if (((u32)RAM_DATA.A + dA_P) > (u32)FLASH_DATA.Amax) RAM_DATA.A = FLASH_DATA.Amax;//  //����� ����� �� ������� �� ������� ������� Amax 
           else RAM_DATA.A = RAM_DATA.A + dA_P;
           
           /*������ �������*/
           TIM4->CCR1 =  RAM_DATA.A; //������ ����
          /* Tim_limit = RAM_DATA.A + FLASH_DATA.TirImpWidth; //������ ���� +������ �������� ����������
           if (Tim_limit > 9800) Tim_limit = 9800;
           TIM4->CCR2 = Tim_limit; //������������ �� ����� 10��
           */
           Tim_limit = RAM_DATA.A + FLASH_DATA.TirImpWidth; //������ ���� +������ �������� ����������
           if (Tim_limit > FLASH_DATA.Amax) Tim_limit = FLASH_DATA.Amax;
           TIM4->CCR2 = Tim_limit; //������������ �� ����� 10��
           

           TIM_Cmd(TIM4, ENABLE); //��� ������ �� ������ ���� � �� ������������ ��������
        }//�� ���� �����
      }//���������� ��������� ����������
      else
      {
        TIM4->CR1 &= ~TIM_CR1_CEN;//���������� ������
         AllThyristorsOFF(); 
      }
        
      EXTI->PR = EXTI_Line11; // ���������� ���� ����������
  } //������ ���������� �� ������

}
  



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
