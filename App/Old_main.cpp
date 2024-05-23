/**
  ******************************************************************************
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "bastypes.h"//основные типы данных, объявление типов структур RAM_DATA и FLASH_DATA
#include "ramdata.h"//глобальные переменные и структура RAM_DATA
#include "init.h"//функции инициализации
#include "App/App.h"
#include "modbus/modbus.h"
#include "DEFINES.h" //все основные, относящиеся только к плате дефайны
/* defines ------------------------------------------------------------------*/
//#define max_drebezg 0x0005//0x2000 // сколько раз нужно проверить нажатие кнопки для подавления дребезга контактов 
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void Fail_Reset(void);
/* Private variables ---------------------------------------------------------*/
    
/**
  * @brief  Main program.
  */
int main(void)              //главная программа
{

  BootLoadCmdFillZero();
  Init();                   //инициализация переферии  

  LED_RUN_ON;
  LED_LINK1_OFF;
  LED_LINK2_OFF;
  LED_ALARM_OFF;
  
  bool start = true;
  
  App::getInstance().run();

}

/*инициализация флагов и переменных предварительная*/
//void Init_soft(void)
//{
//    /*дополнительная инициализация софта, которую потом отдельной цункцией запилить*/
//  //все флаги вхо и вых обнулить
//  RAM_DATA.FLAGS.BA.DI1 = 0;
//  RAM_DATA.FLAGS.BA.DI2_BURNING = 0;
//  RAM_DATA.FLAGS.BA.DI3_STOP = 0;
//  RAM_DATA.FLAGS.BA.DI4_START = 0;
//  RAM_DATA.FLAGS.BA.DI5_TERM = 0;
//  RAM_DATA.FLAGS.BA.DOUT1_FAIL = 0;
//  RAM_DATA.FLAGS.BA.DOUT2_SHAKE = 0;
//  RAM_DATA.FLAGS.BA.DOUT3_SIGN = 0;
//  
//  RAM_DATA.FLAGS.BA.DOUT4_SHAKE2 = 0;
//  RAM_DATA.FLAGS.BA.DOUT5 = 0;
//  RAM_DATA.FLAGS.BA.DOUT6 = 0;
//  RAM_DATA.FLAGS.BA.DI6 = 0;
//  
//  RAM_DATA.FLAGS.BA.A_STOP = 0; // флаг повышения угла перед остановкой
//  RAM_DATA.FLAGS.BA.WAIT = 0;//сброс флага состояния ожидания
//  RAM_DATA.FLAGS.BA.SPARK = 0;//сброс флага искры
//  RAM_DATA.FLAGS.BA.TR_ENABLE =0; //запретили включение тиристоров - разрешать при кнопке Пуск
//  RAM_DATA.FLAGS.BA.TR1 = 0;//сброс флага тиристоров
//  RAM_DATA.FLAGS.BA.TR2 = 0;//сброс флага тиристоров
//        
//             
//}
////сброс всех аварий
//void Fail_Reset(void)
//{
//  RAM_DATA.FLAGS.BA.SYNCF = 0;
//  RAM_DATA.FLAGS.BA.MTZ_SH = 0;
//  RAM_DATA.FLAGS.BA.MTZ_L = 0;
//  RAM_DATA.FLAGS.BA.SHC = 0;
//  RAM_DATA.FLAGS.BA.OPC = 0;
//  RAM_DATA.FLAGS.BA.OVH = 0;
//  RAM_DATA.FLAGS.BA.THFOC = 0;
//  RAM_DATA.FLAGS.BA.THFAO = 0;
//  RAM_DATA.FLAGS.BA.FQS = 0;
//  
//  RAM_DATA.FLAGS.BA.DOUT1_FAIL = 0;
//  RAM_DATA.FLAGS.BA.DOUT3_SIGN = 0; //зажгли светик АВАРИЯ
//}


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
