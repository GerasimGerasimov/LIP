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
#include "init.h"

/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define _175grad 0xAA79


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static u8 num_avg = 0;
//static u32 E_Uload = 0;
//static u32 E_Iload = 0;
//static u32 E_Ish = 0;

//static u8 num_avg_Efiltr = 0;
//static u32 Efiltr_Uload = 0;
//static u32 Efiltr_Iload = 0;


//static u16 dA = 0; //на сколько отбрасывать Альфа
//static u16 A_NormMode = 0; //значение отброшенного угла = RAM_DATA.A +dA
 u8 Pause_Imp = 0; //сколько импульсов осталось подождать перед запуском
 bool NormalMode = TRUE; //если пришла искра и скорость надо большую для сбрасывания угла - false

//static u16 Shake_cnt = 0; //ширина имупльса включения встряхивателя
//static u32 Shake_stop_cnt = 0; //часть периода встряхивателя, пока он не включен

//static u8 SYNC_cnt = 0; //ширина имупльса включения встряхивателя
 u16 SYNC_Yes =0; //срабатывать только на нужную синхру, отбрасывать джиттер

//дла проверки на частую искру
//static bool SPARK = FALSE; //нет искры - фалсе
//static u8 SPARK_cnt = 0; //счетчик искр - в полупериод была исрка или нет
//static u8 SP_buf_cnt = 0; //счетчик записи/проверки SPARK_cnt в буфер и проверки на аварию
//u8 Spark_Pause_Imp=0; //akfu отсчета полупериодов - сколько не делать проверку на мтз 
  
//static u8 FAIL_flgcnt = 0; //сколько держать замкнутым контактор, флаг аварии
//u16 Il_buf[100]; 
//u16 Ul_buf[100];
//u8 i_buf_main=0; //счетчик для записи осцил
/* Private function prototypes -----------------------------------------------*/
// дефайны перед функцией обработки DMA. Параметры аналоговой схемы
/*#define Uth 208
//#define Ith 150
//#define Vcc 2087
//#define R1 26
//#define R2 100
//#define R3 (220+150)
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


//дма
void DMA1_Channel3_IRQHandler(void)//прерывание вызывается, когда переданы все данные в SPI
{ 
  ++RAM_DATA.counter1;
  if (DMA_GetITStatus(DMA1_IT_TC3)){
    DMA_Cmd(DMA1_Channel3, DISABLE);

    //ожидание, пока SPI закончит отправку
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){
      ++RAM_DATA.counter3;
    };

    GPIO_SetBits(GPIOA, GPIO_Pin_6); //ON
    GPIO_ResetBits(GPIOA, GPIO_Pin_6); //OFF

    fillArr();

    DMA_ClearITPendingBit(DMA1_IT_TC3);
    //DMA1->IFCR |= DMA_ISR_TCIF3; //сбрасываем флаг окончания обмена  
    DMA1_Channel3->CNDTR = 15;

    DMA_Cmd(DMA1_Channel3, ENABLE);
    
    ++RAM_DATA.counter2;
  }
}


/*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{

    if (EXTI->PR & EXTI_Line11)  
    {  
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
