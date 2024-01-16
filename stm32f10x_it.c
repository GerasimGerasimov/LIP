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
#include "modbus/uart2rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS (клиент)
#include "crc16.h"
#include "ramdata.h"
#include "flashdata.h"

#include "DEFINES.h" //все основные, относящиеся только к плате дефайны
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


static u16 dA = 0; //на сколько отбрасывать Альфа
static u16 A_NormMode = 0; //значение отброшенного угла = RAM_DATA.A +dA
 u8 Pause_Imp = 0; //сколько импульсов осталось подождать перед запуском
 bool NormalMode = TRUE; //если пришла искра и скорость надо большую для сбрасывания угла - false

static u16 Shake_cnt = 0; //ширина имупльса включения встряхивателя
static u32 Shake_stop_cnt = 0; //часть периода встряхивателя, пока он не включен

static u8 SYNC_cnt = 0; //ширина имупльса включения встряхивателя
 u16 SYNC_Yes =0; //срабатывать только на нужную синхру, отбрасывать джиттер

//дла проверки на частую искру
static bool SPARK = FALSE; //нет искры - фалсе
static u8 SPARK_cnt = 0; //счетчик искр - в полупериод была исрка или нет
static u8 SP_buf_cnt = 0; //счетчик записи/проверки SPARK_cnt в буфер и проверки на аварию
u8 Spark_Pause_Imp=0; //akfu отсчета полупериодов - сколько не делать проверку на мтз 
  
static u8 FAIL_flgcnt = 0; //сколько держать замкнутым контактор, флаг аварии
u16 Il_buf[100]; 
u16 Ul_buf[100];
u8 i_buf_main=0; //счетчик для записи осцил
/* Private function prototypes -----------------------------------------------*/
// дефайны перед функцией обработки DMA. Параметры аналоговой схемы
/*#define Uth 208
#define Ith 150 //* 10% от номинального тока 
#define Vcc 2087//1200
#define R1 26
#define R2 100
#define R3 (220+150) //* здесь добавочный резистор от 0 до 150 
*/
/* Private functions ---------------------------------------------------------*/



/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


void SysTickHandler(void)
{
 
}

//модбас прерывания
void TIM1_CC_IRQHandler (void)
{
  if ((TIM1->SR & TIM_FLAG_CC1)&&(TIM1->DIER & TIM_IT_CC1)) TIM1_user_U1();
  if ((TIM1->SR & TIM_FLAG_CC2)&&(TIM1->DIER & TIM_IT_CC2)) TIM1_user_U2();    
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
*******************************************************************************/
//шим ключом тиристоров
void TIM2_IRQHandler(void)
{
   TIM2->SR = 0;
}

//управление тиристорами - угол и шим
void TIM4_IRQHandler(void)
{
   
 //  if ((TIM4->SR & TIM_FLAG_CC1)&&(TIM4->DIER & TIM_IT_CC1)) 
  if (TIM4->SR & TIM_IT_CC1)
   {
     TIM4->CR1 &= ~TIM_CR1_CEN;//остановить таймер
     TIM4->SR = 0;     
     TxThyristorON(); //отсчитался угол, включаем шим - импульс управления тиристором
     TIM4->CR1 |= TIM_CR1_CEN;//TIMER1_START;
     
   }
 //  if ((TIM4->SR & TIM_FLAG_CC2)&&(TIM4->DIER & TIM_IT_CC2)) 
  if (TIM4->SR & TIM_IT_CC2)
   { 
     TIM4->CR1 &= ~TIM_CR1_CEN;//остановить таймер
     TIM4->SR = 0;
     AllThyristorsOFF(); //импульс кончился, выкл шим    
    
   }
   
   
}


//дма работа с ацп - каждые 0,1 мс
void DMA1_Channel1_IRQHandler(void)//прерывание вызывается, когда считаны все данные с АЦП, калибруем и получаем средние значения
{ 
  static u8 i_buf=0; //счетчик для записи осцил
  static u8 i_buf_100ms=0; //счетчик для записи осцил
  // Дополнительные переменные для модуля искрового разряда
  static s32 Ec=0; // Накопитель, эмулирует заряд конденсатора
  s32 Uc =Ec/1000;
  s32 ic=0;

  
//RAM_DATA.V_Ref = ADC1->JDR1; //скопировали референс

  SYNC_Yes++; //счетик нужной синхры
  /*калибровка нуля и калибровка наклона,значения относительно Vref*/
   if(RAM_DATA._Uload < FLASH_DATA.Uload_offs) RAM_DATA._Uload = FLASH_DATA.Uload_offs; //ограничение по низу нулем
   if(RAM_DATA._Iload < FLASH_DATA.Iload_offs) RAM_DATA._Iload = FLASH_DATA.Iload_offs; //ограничение по низу нулем
   if(RAM_DATA._Ish < FLASH_DATA.Ish_offs) RAM_DATA._Ish = FLASH_DATA.Ish_offs; //ограничение по низу нулем
   RAM_DATA.Uload = ((u32)(RAM_DATA._Uload - FLASH_DATA.Uload_offs)* FLASH_DATA.KADC_Uload) / RAM_DATA.V_Ref;
   RAM_DATA.Iload = ((u32)(RAM_DATA._Iload - FLASH_DATA.Iload_offs) * FLASH_DATA.KADC_Iload) / RAM_DATA.V_Ref;
   RAM_DATA.Ish = ((u32)(RAM_DATA._Ish - FLASH_DATA.Ish_offs)* FLASH_DATA.KADC_Ish) / RAM_DATA.V_Ref;
   
  /*проверка на искру - делается только когда нет Прожига*/
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
       //Если ток больше порога 0...10%, значит считаем искру
       if (RAM_DATA.Iload > FLASH_DATA.Ith) //пришла искра!
       {
         RAM_DATA.FLAGS.BA.SPARK = 1; //флаг прихода искры дла EXTI 
         RAM_DATA.Spark_cnt ++; //счетчик искры для EXTI
         SPARK = TRUE; //флаг искры для проверки на частую искру
       }
     } 
     Ec+=ic/10;
   }
   /*для записи данных в осцилл*/
   if (RAM_DATA.load_buf==0)
   {
      Il_buf[i_buf] = RAM_DATA.Iload;
      Ul_buf[i_buf] = RAM_DATA.Uload;
      i_buf++;
      if (i_buf==100)i_buf=0;
   }
  /*для усреднения за полпериода*/
   E_Uload += RAM_DATA.Uload;
   E_Iload += RAM_DATA.Iload;
   E_Ish += RAM_DATA.Ish;
   num_avg ++;
   if (num_avg == 100) //каждые 10мс
   {
     /*для записи осцилла в аджастер*/
     if (i_buf_100ms == 10)
     { i_buf_100ms =0;
        i_buf_main++;
        if (i_buf_main ==100) i_buf_main=0;
     }
     else i_buf_100ms ++;
     /*усреднения за полпериода*/  
      RAM_DATA.Uload_avg = ((u32)E_Uload/num_avg);
      RAM_DATA.Iload_avg = ((u32)E_Iload/num_avg);
      RAM_DATA.Ish_avg = ((u32)E_Ish/num_avg);
      E_Uload = 0; //обнулили переменные стандартного усреднения дма
      E_Iload = 0 ;
      E_Ish = 0;
      num_avg =0;
      
      /*для красивого отображения на дисплее*/
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
      

      
      /*проверка на аварии*/
     /* if (RAM_DATA.FLAGS.BA.SPARK == 0)//если в этот раз не было искры - проверять на mtz 
      {
        Ish_mtz_fail(); // mtz по входу
        Iload_mtz_fail(); //mtz по нагрузке
      }*/

      /*проверять когда нормальный режим работы и когда нет остановки и стопа*/
      if (NormalMode == TRUE)//если нормальный режим работы
      {
        if (RAM_DATA.FLAGS.BA.A_STOP == 0)//и если нет остановки
        {
          if (RAM_DATA.FLAGS.BA.WAIT == 0)//и если система работает, не стоит
          {
            SC_fail(); //кз
            OC_fail(); //хх
          }
        }
      }
      
      /*проверка на частую искру*/
      if (SPARK == TRUE)//каждые 10 мc, если в этом полупериоде была искра - весь полупериод держится переменная и сбрасывается в усреднении
      {
        SPARK = FALSE;
        SPARK_cnt++;
      }
      else //если в этом полупериоде не было искры - флаг не устанвавливался и можно проверять mtz
      {
        /*проверка на аварии*/
        // if (RAM_DATA.FLAGS.BA.SPARK == 0)//и прошло три полупериода после сброса искры
        if (Spark_Pause_Imp == 0)//и прошло три полупериода после сброса искры
        {
          Ish_mtz_fail(); // mtz по входу
          Iload_mtz_fail(); //mtz по нагрузке
        }
      }
      
      if (SP_buf_cnt == 10)//каждые 100мс
      {
        SPARK_fail(SPARK_cnt); //проверка аварии на частую искру
        SPARK_cnt= 0;
        SP_buf_cnt = 0;
      }
      else SP_buf_cnt++;
     
      /* проверка на аварию - нет синхронизации в течение 100 мс*/
      SYNC_cnt++; //счетчик авария синхры 1 тик = 10 мс
      if (SYNC_cnt >= SYNC_fail_cnt) RAM_DATA.FLAGS.BA.SYNCF = 1; //выставили флаг аварии синхры 
      
     /*включение/выключение встряхивателя*/
      if(RAM_DATA.FLAGS.BA.TR_ENABLE == 1) //если разрешен нормальный рабочий режим
      {
        if (RAM_DATA.FLAGS.BA.DOUT2_SHAKE) //если встряхиватель вкл
        {  
          if (Shake_cnt) Shake_cnt--; 
          else //встряхиватель отработал
          {
            Shake_stop_cnt = ((FLASH_DATA.ShPeriod - FLASH_DATA.ShWidth)); //заряжаем встряхиватель на счет секунд
            RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 0;//выкл. встряхиватель
          }
        }
        else //встряхиватель выключен
        { 
           if (Shake_stop_cnt) Shake_stop_cnt--; 
          else //время простоя встряхивателя закончилось
          {
            Shake_cnt = (FLASH_DATA.ShWidth); //заряжаем встряхиватель на счет секунд
            RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 1;//вкл. встряхиватель
          }
        }   
      }
      else
      {
        if (RAM_DATA.FLAGS.BA.DOUT2_SHAKE) //если встряхиватель вкл
        {  
          if (Shake_cnt) Shake_cnt--; 
          else //встряхиватель отработал
          {
            Shake_stop_cnt = 0; //заряжаем встряхиватель на счет секунд
            RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 0;//выкл. встряхиватель
          }
        }
        else{ //если выкл - все просто обнулить
          Shake_cnt=0;
          Shake_stop_cnt=0;
        }
      }
      
      /*ограничение времени замкнутого контактора по аварии*/
      if(RAM_DATA.FLAGS.BA.DOUT1_FAIL ==1)
      {
        if(FAIL_flgcnt == 100)
        { 
          FAIL_flgcnt =0;
          RAM_DATA.FLAGS.BA.DOUT1_FAIL =0;
        }
        else {FAIL_flgcnt++;}
      }
      
      
   }//усреднение каждые 10мс 

      
   DMA1->IFCR |= DMA_ISR_TCIF1; //сбрасываем флаг окончания обмена  
}


/*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
    s16 error = 0; //для П-регулятора
    u32 dA_P = 0; //дельта альфа, найденная из П-регулятора   
    u16 Tim_limit = 0;// ограничение для таймера, чтобы не выходил за границу полупериода синхры
    if (EXTI->PR & EXTI_Line11)  // если пришла синхра
    { 
      if (SYNC_Yes <90) //если пришла ложная синхра, частота много меньше 50Гц
      {
        EXTI->PR = EXTI_Line11; // сбрасываем флаг прерывания
        return; //выходим
      }
      SYNC_Yes = 0; //обнулили для последующей проверки
      SYNC_cnt=0;
      if(RAM_DATA.FLAGS.BA.TR_ENABLE == 1) //если разрешено управлять тиристорами - управляем!!
      {
        /*остановка таймера и выкл всего*/
        TIM4->CR1 &= ~TIM_CR1_CEN;//остановить таймер
        TIM4->SR = 0;
        TIM4->CNT = 0; //обнулили, теперь считать с 0
        AllThyristorsOFF(); //выключить тиристоры в любом случае */
        
        /*если у нас в прошлый полупериод была искра*/
        if (RAM_DATA.FLAGS.BA.SPARK == 1) 
        {  
          RAM_DATA.load_buf = 1;   //говорим, что в буферы нельзя писать осцил       
          RAM_DATA.FLAGS.BA.SPARK=0;
          NormalMode = FALSE; 
          Pause_Imp = FLASH_DATA.SkipImpSpark; //сколько импульсов ждать перед запуском 
          dA = ((u32)RAM_DATA.Spark_cnt * FLASH_DATA.K_SensSpark); //на сколько отбрасывать альфа
          
          RAM_DATA.Spark_cnt = 0; //обнулили счетчки длительности искры
          A_NormMode = RAM_DATA.A +dA; //записали размер угла, до которого отбрасываем
          RAM_DATA.Spark_cnt_view = dA;//пиковый детектор ДЛЯ ПРОВЕРКИ
          //проверили размер отбрасывания
          if (A_NormMode > (FLASH_DATA.Amax)) A_NormMode = FLASH_DATA.Amax; //чтобы суммарный отброшенный альфа не выходил на верхнюю границу Amax
          if (A_NormMode < (FLASH_DATA.Amin)) A_NormMode = FLASH_DATA.Amin; //чтобы суммарный отброшенный альфа не выходил на верхнюю границу Amax
          Pause_Imp --; //один этот импульс уже пропускаем, так что -1
          Spark_Pause_Imp=1;
        }//была искра
        else
        {
          if (RAM_DATA.FLAGS.BA.A_STOP ==1) //если надо остановить систему - увеличиваем угол А
          {
             RAM_DATA.A = RAM_DATA.A + FLASH_DATA.VA_rise; //увеличиваем угол до максимального значение Amax
             if (RAM_DATA.A > FLASH_DATA.Amax ) 
             {
               RAM_DATA.A = FLASH_DATA.Amax ; // записали в размер используемого угла Amax ?надо ли?
               RAM_DATA.FLAGS.BA.A_STOP = 0; // сбросили флаг повышения угла перед остановкой
               RAM_DATA.FLAGS.BA.WAIT = 1;//установили  флага состояния ожидания
               RAM_DATA.FLAGS.BA.TR_ENABLE =0; //запретили включение тиристоров - разрешать при кнопке Пуск
               EXTI->PR = EXTI_Line11; // сбрасываем флаг прерывания
               return; //выходим
             }
          }
          else //нормальный режим работы
          {
             /*отбрасывание и уменьшение угла*/
             if (NormalMode==FALSE) //пока что используемый угол больше A_NormMode, скорость спада угла высокая
             { 
               if (Pause_Imp > 0 )//если задержка не кончилась 
               { 
                  Pause_Imp --; //уменьшаем счетчик
                  if (Pause_Imp==0) RAM_DATA.A = FLASH_DATA.Amax ; // записали в размер используемого угла Amax
                  EXTI->PR = EXTI_Line11; // сбрасываем флаг прерывания
                  return; //выходим
               }  
               Spark_Pause_Imp=0; //прошло три периода задержки
               //задержка кончилась               
              if ((u32)RAM_DATA.A < ((u32)FLASH_DATA.VA_fall_spark+ (u32)A_NormMode))
              { RAM_DATA.A = A_NormMode;// //чтобы альфа не выходил за границу A_NormMode
                if (RAM_DATA.A <(A_NormMode+1)) NormalMode = TRUE; //если дошли до отбрасываемой величины - установить нормальный режим, нормальную скорость спада
              }
              else  RAM_DATA.A = RAM_DATA.A - FLASH_DATA.VA_fall_spark; //быстро снижаем угол до A_NormMode 
             
             }  
             else  {RAM_DATA.A = RAM_DATA.A - FLASH_DATA.VA_fall_norm;} //нормальный режим - снижаем с нормальной скоростью
             if (RAM_DATA.A < FLASH_DATA.Amin ) RAM_DATA.A = FLASH_DATA.Amin;// //чтобы альфа не выходил на нижнюю границу Amin
           }   
           /*П-регулятор*/
           error = (s32)RAM_DATA.Iload_avg - (s32)RAM_DATA.Iz; //FLASH_DATA.Iz;
           if (error > 0) dA_P = error * FLASH_DATA.Ky_I;  
           error = (s32)RAM_DATA.Uload_avg - (s32)RAM_DATA.Uz; //FLASH_DATA.Uz;
           if (error > 0) dA_P += (error * FLASH_DATA.Ky_U);
          // RAM_DATA.A = RAM_DATA.A + dA_P;
           if (((u32)RAM_DATA.A + dA_P) > (u32)FLASH_DATA.Amax) RAM_DATA.A = FLASH_DATA.Amax;//  //чтобы альфа не выходил на верхнюю границу Amax 
           else RAM_DATA.A = RAM_DATA.A + dA_P;
           
           /*запуск таймера*/
           TIM4->CCR1 =  RAM_DATA.A; //длинна угла
          /* Tim_limit = RAM_DATA.A + FLASH_DATA.TirImpWidth; //длинна угла +длинна импульса управления
           if (Tim_limit > 9800) Tim_limit = 9800;
           TIM4->CCR2 = Tim_limit; //ограничиваем на почти 10мс
           */
           Tim_limit = RAM_DATA.A + FLASH_DATA.TirImpWidth; //длинна угла +длинна импульса управления
           if (Tim_limit > FLASH_DATA.Amax) Tim_limit = FLASH_DATA.Amax;
           TIM4->CCR2 = Tim_limit; //ограничиваем на почти 10мс
           

           TIM_Cmd(TIM4, ENABLE); //вкл таймер на отсчет угла и на длительность импульса
        }//не было искры
      }//разрешение включения тиристоров
      else
      {
        TIM4->CR1 &= ~TIM_CR1_CEN;//остановить таймер
         AllThyristorsOFF(); 
      }
        
      EXTI->PR = EXTI_Line11; // сбрасываем флаг прерывания
  } //пришло прерывание по синхре

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
