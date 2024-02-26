/**
  ******************************************************************************
  * @file    intmash\projects\CHG.CTRL.CPU\firmware\CHG.CTRL.CPU
  * @author  Sledin A.S. (Следин А.С.)
  * @version V0.0.2
  * @date    30/04/2013
  * @brief   Main program body.
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"//библиотека STM
#include "bastypes.h"//основные типы данных, объявление типов структур RAM_DATA и FLASH_DATA
#include "ramdata.h"//глобальные переменные и структура RAM_DATA
#include "flashdata.h"//глобальные константы и структура FLASH_DATA
#include "crc16.h"//модуль контрольной суммы
#include "init.h"//функции инициализации

#include "fram/fram.h"
#include "modbus/uart1rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS1 (клиент)
#include "modbus/uart2rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS2 (клиент)

#include "DEFINES.h" //все основные, относящиеся только к плате дефайны
#include "modbus/modbus.h"
#include "stm32f10x_it.h"
#include "bootloader.h"

    
/* defines ------------------------------------------------------------------*/
#define max_drebezg 0x0005//0x2000 // сколько раз нужно проверить нажатие кнопки для подавления дребезга контактов 
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    
    
/**
  * @brief  Main program.
  */
int main(void)              //главная программа
{

  GPIO_INIT_Configuration();

  if (INIT_IST == 0) {
    LED_ALARM_OFF;
    
    if (isBootLoaderMustBeStart() != true) {
      if (isApplicationReadyToStart()) {
          jumpToApplication();
      }
    }
  }

  Init();                   //инициализация переферии  


  LED_RUN_OFF;
  LED_LINK1_OFF;
  LED_LINK2_OFF;
  LED_ALARM_ON;

  
  while (1)//основной цикл программы
  {    
    if ((U1_SwCNT()) ||(U2_SwCNT()))//смотрим пришел ли запрос по Модбасу и 1 и 2 сразу смотрим для проверки
    {
      if (LED_LINK1_ST) LED_LINK1_ON;
      else LED_LINK1_OFF; 
      
    }
     
  }
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
