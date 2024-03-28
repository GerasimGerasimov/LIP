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
#include "modbus/uart1rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS (клиент)
#include "crc16.h"
#include "ramdata.h"
#include "flashdata.h"
#include "livecontrol.h"

/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

// CE - выбор сдвигового регистра
// LOCK - защёлка

// ST - состояние сигнала
// UP - выставить сигнал в 1
// DWN - выставить сигнал в 0  

#define DI_CE_ST     (GPIOB->ODR & GPIO_Pin_15)
#define DI_CE_UP     GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define DI_CE_DWN    GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define DI_LOCK_ST   (GPIOB->ODR & GPIO_Pin_12)
#define DI_LOCK_UP   GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define DI_LOCK_DWN  GPIO_ResetBits(GPIOB, GPIO_Pin_12)


/* Private variables ---------------------------------------------------------*/
u16 SPI_DIO_Inputs;



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
  //if ((TIM1->SR & TIM_FLAG_CC2)&&(TIM1->DIER & TIM_IT_CC2)) TIM1_user_U2();    
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

//считывание DI с кнопок
u8 SPI_DIO_Processing()
{
  u8 RetVal = 0;
  static bool isWaitReceive = false;
  //если кристалл ещё не выбран CE в "1"
  //сдвиговые регистры входов находятся в ресете
  //инициализируем работу сдвиговых регистров
  if (DI_CE_ST) {
      //если чип 74HC165 ещё не выбран, то сначала проверяю, в каком состоянии защёлка
      //если защёлка не в нуле, то опускаю защёлку чтобы входы перешли в сдвиговый регистра
      if (DI_LOCK_ST) {//если защёлка в "1" 

        DI_LOCK_DWN;//то ставлю в "0" на этом этапе денные из параллельного регистра переходят в последовательный
      } else { //если защёлка в "0"

        DI_LOCK_UP;//то ставлю её в "1" (т.е. возвращаю в исходное состояние)
        DI_CE_DWN;//и выбираю 74HC165
        isWaitReceive = false;
      }
  }
  else {//чип 74HC165 уже выбран
    if (!isWaitReceive) {//если ещё не жду отправки (с параллельным приёмом!)

      SPI2->DR = 0xF0F0;// //то оптправить по SPI единицы, чтобы в ответ получить состояние дискретных входов
      isWaitReceive = true;
    } else {

      //RAM_DATA.UoutAve++;
      //прошло 0.001 сек, за это время данные должны быть получены
      DI_CE_UP;//"освобождаю" кристалл
      DI_LOCK_UP;
      isWaitReceive = false;
      
      RAM_DATA.DI = ~(SPI2->DR);
      /* (InputsPolarity == DIO_MODE_NORMAL)
                          ? SPI_DIO->DR
                          : ~(SPI_DIO->DR); */
    }
  }  
  return RetVal;
}

//таймер для SPI2 дискретных входов
void TIM4_IRQHandler(void)
{
  TIM4->SR = 0;
  
   SPI_DIO_Processing();   
  ctrlSysLive();
    
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
