/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"//библиотека STM
#include "bastypes.h"//основные типы данных, объявление типов структур RAM_DATA и FLASH_DATA
#include "ramdata.h"//глобальные переменные и структура RAM_DATA
#include "flashdata.h"//глобальные константы и структура FLASH_DATA
#include "crc16.h"//модуль контрольной суммы
#include "init.h"//функции инициализации

#include "modbus/uart1rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS1 (клиент)

#include "DEFINES.h" //все основные, относящиеся только к плате дефайны
#include "modbus/modbus.h"
#include "DevicePollManager/Slot.h"
#include "DevicePollManager/DevicePollManager.h"

#include <vector>
    
/* defines ------------------------------------------------------------------*/
#define max_drebezg 0x0005//0x2000 // сколько раз нужно проверить нажатие кнопки для подавления дребезга контактов 
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void parseRespond(Slot* slot, u8* reply){
  RAM_DATA.data[1] = reply[0];
  RAM_DATA.data[2] = reply[1];
  RAM_DATA.data[3] = reply[2];
  RAM_DATA.data[4] = reply[3];
  RAM_DATA.data[5] = reply[4];
  RAM_DATA.data[6] = reply[5];
  RAM_DATA.data[7] = reply[6];
}
/**
  * @brief  Main program.
  */
int main(void)              //главная программа
{

  Init();                   //инициализация переферии  

  LED_RUN_ON;
  LED_LINK1_OFF;
  LED_LINK2_OFF;
  LED_ALARM_OFF;

  //Slot* slot = new Slot;
  //std::vector<u8> command = {0x01, 0x10, 0x00, 0x06, 0x00, 0x01, 0x02, 0x00, 0x55 };
  //usart2DMA_init(slot->InputBuf);
  //slot->addcmd(command);
  //TxDMA1Ch7(slot->cmdLen, slot->OutBuf);
  //slot->TimeOut = 1000;
  //slot->onData = parseRespond;
  //DevicePollManager::getInstance().addSlot(slot);

  while (1)//основной цикл программы
  {    
    if (U1_SwCNT())//смотрим пришел ли запрос по Модбасу и 1 сразу смотрим для проверки
    {
      if (LED_LINK1_ST) LED_LINK1_ON;
      else LED_LINK1_OFF; 
      
    }
    DevicePollManager::getInstance().execute();
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
