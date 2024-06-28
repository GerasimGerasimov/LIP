/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "DEFINES.h"
#include "ramdata.h"
#include "livecontrol.h"
/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

u16 SPI_DIO_Inputs;

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
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

void TIM1_CC_IRQHandler(void){
  if((TIM1->SR & TIM_FLAG_CC1) && (TIM1->DIER & TIM_IT_CC1)) TIM1_user_U1();

}

//считывание DI с кнопок
void SPI_DIO_Processing(){
  static bool isWaitReceive = false;
  //если кристалл ещё не выбран CE в "1"
  //сдвиговые регистры входов находятся в ресете
  //инициализируем работу сдвиговых регистров
  if(DI_CE_ST){
      //если чип 74HC165 ещё не выбран, то сначала проверяю, в каком состоянии защёлка
      //если защёлка не в нуле, то опускаю защёлку чтобы входы перешли в сдвиговый регистра
    if(DI_LOCK_ST){//если защёлка в "1" 

      DI_LOCK_DWN;//то ставлю в "0" на этом этапе денные из параллельного регистра переходят в последовательный
    }
    else{ //если защёлка в "0"

      DI_LOCK_UP;//то ставлю её в "1" (т.е. возвращаю в исходное состояние)
      DI_CE_DWN;//и выбираю 74HC165
      isWaitReceive = false;
    }
  }
  else{//чип 74HC165 уже выбран
    if(!isWaitReceive){//если ещё не жду отправки (с параллельным приёмом!)

      SPI2->DR = 0xFFFF;// //то оптправить по SPI единицы, чтобы в ответ получить состояние дискретных входов
      isWaitReceive = true;
    }
    else{
   //на всякий случай
      while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET){

      };
        //RAM_DATA.UoutAve++;
        //прошло 0.001 сек, за это время данные должны быть получены
      DI_CE_UP;//"освобождаю" кристалл
      DI_LOCK_UP;
      isWaitReceive = false;

      SPI_DIO_Inputs = ~(SPI_I2S_ReceiveData16(SPI2));
      /* (InputsPolarity == DIO_MODE_NORMAL)
                          ? SPI_DIO->DR
                          : ~(SPI_DIO->DR); */

    }
  }
}

void TIM2_IRQHandler(){
  TIM2->SR = 0;

  SPI_DIO_Processing();
  RAM_DATA.DI = SPI_DIO_Inputs;
  ctrlSysLive();
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void){}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void){
  /* Go to infinite loop when Hard Fault exception occurs */
  while(1){
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void){}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void){}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void){}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
