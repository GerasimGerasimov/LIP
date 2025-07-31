/* Includes ------------------------------------------------------------------*/
#include "bastypes.h"//основные типы данных, объявление типов структур RAM_DATA и FLASH_DATA
#include "ramdata.h"//глобальные переменные и структура RAM_DATA
#include "init.h"//функции инициализации
#include "App/App.h"
#include "modbus/modbus.h"
#include "DEFINES.h" //все основные, относящиеся только к плате дефайны
#include "id.h"

/* defines ------------------------------------------------------------------*/
//#define max_drebezg 0x0005//0x2000 // сколько раз нужно проверить нажатие кнопки для подавления дребезга контактов 
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  Main program.
  */

 const char IDtext[] =" LIP v1.4.0 31.07.2025 www.intmash.ru ";

int main(void)              //главная программа
{
    BootLoadCmdFillZero();// Очистка памяти команд Загрузчика (если есть Bootloader) 
    Init();// Инициализация переферии  
    IDinit(IDtext);
    LED_RUN_ON;
    LED_LINK1_OFF;
    LED_LINK2_OFF;
    LED_ALARM_OFF;

    App::getInstance().run();

}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line){
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

       /* Infinite loop */
    while(1){
    }
}
#endif


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
