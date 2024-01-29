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
#include "lip/tirist.h"
#include "lip/fails.h"
#include "modbus/uart1rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS1 (клиент)
#include "modbus/uart2rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS2 (клиент)

#include "DEFINES.h" //все основные, относящиеся только к плате дефайны
#include "modbus/modbus.h"
#include "spi.h"

#include "stm32f10x_it.h"

    
/* defines ------------------------------------------------------------------*/
#define max_drebezg 0x0005//0x2000 // сколько раз нужно проверить нажатие кнопки для подавления дребезга контактов 
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
int main(void)              //главная программа
{

  Init();                   //инициализация переферии  
  uint8_t arr[5] = {1, 2, 4, 8, 16};
  
  LED_RUN_ON;
  LED_LINK1_OFF;
  LED_LINK2_OFF;
  LED_ALARM_OFF;
  /*дополнительная инициализация софта, которую потом отдельной цункцией запилить*/
  Init_soft();// тут сброс всего в  начальное значение
  Fail_Reset();//сброс флагов аварий

  while (1)//основной цикл программы
  {    
    if ((U1_SwCNT()) ||(U2_SwCNT()))//смотрим пришел ли запрос по Модбасу и 1 и 2 сразу смотрим для проверки
    {
      if (LED_LINK1_ST) LED_LINK1_ON;
      else LED_LINK1_OFF; 
    }
        
    spi_send(~((uint8_t)RAM_DATA.Iz));
    //SPI1->C1
    //записали значения в дискретные выходы 
//    if (RAM_DATA.FLAGS.BA.DOUT1_FAIL) GPIOA->BRR = GPIO_Pin_14; //если 1 - выход мк роняем в 0, на выходе платы 1
//    else GPIOA->BSRR = GPIO_Pin_14;  //авария
//    if (RAM_DATA.FLAGS.BA.DOUT2_SHAKE) GPIOA->BRR = GPIO_Pin_13; 
//    else GPIOA->BSRR = GPIO_Pin_13;  //встряхиватель пишем его состояние
//    if (RAM_DATA.FLAGS.BA.DOUT3_SIGN) GPIOA->BRR = GPIO_Pin_12; //если 1 - выход мк роняем в 0, на выходе платы 1
//    else GPIOA->BSRR = GPIO_Pin_12; //сигнализация
    //НАДО ПИСАТЬ ВТОРОЙ ВСТРЯХИВАТЕЛЬ
    
//считали дискретные входы  
// RAM_DATA.FLAGS.BA.DI4_START = !(GPIOB->IDR & GPIO_Pin_3); //кнопка ПУСК
// RAM_DATA.FLAGS.BA.DI3_STOP = !(GPIOB->IDR & GPIO_Pin_4); //кнопка СТОП/деблокировка защит
// RAM_DATA.FLAGS.BA.DI5_TERM = (GPIOA->IDR & GPIO_Pin_15); //термореле OVH
// RAM_DATA.FLAGS.BA.DI2_BURNING = !(GPIOC->IDR & GPIO_Pin_15); //тумблер прожига
 
    /*проверка на аварию - Перегрев*/
//    OVH_fail(); 
    //считали состояние тиристоров - нафига?...
//    RAM_DATA.FLAGS.BA.TR2      = (GPIOB->IDR & GPIO_Pin_0);//GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
//    RAM_DATA.FLAGS.BA.TR1      = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1); //так работает, а как для тир2 - нет работает хз почему
    
///    if (FLASH_DATA.Amin >= FLASH_DATA.Amax) //защита от дурака, если у нас Amin кто-то выставил больше Amzx - это плохо
//    {
      
//      RAM_DATA.FLAGS.BA.DI3_STOP=1;
//    }
    
    /*проверка на режимы работы и включеные кнопки*/
//    if (RAM_DATA.FLAGS.BA.DI4_START == 1) //Обнаружили нажатой кнопку пуск
//    {    
//      START();
//    }
//     if (RAM_DATA.FLAGS.BA.DI3_STOP == 1) //Обнаружили нажатой кнопку стоп/деблокировка
//    {
//      Norm_STOP = 1; //нажали кнопку стоп
//      STOP();
//    }
    

    
//    if (RAM_DATA.FLAGS.BA.TR_ENABLE == 1) Fail_Check(); //если работает нормально - проверяем постоянно на ошибки
    
    /*проверка на - нажата ли кнопка СТОП первый раз*/
//    if (RAM_DATA.FLAGS.BA.WAIT == 1) //уже система остановилась
//    {
//      if (Norm_STOP == 1) //была нажата кнопка СТОП для остановки, а не для деблокировки
//      {
        // записываем параметр в базу
//        ModbusFlashWrite_DATA(RAM_DATA.Iz, RAM_DATA.Uz);
//        Norm_STOP = 0; // обнулили, чтобы больше 1 раза не записывалось 
 //     }
//    }
    
//    if (RAM_DATA.load_buf !=0) //пишем данные в аджастер для просмотра осцилла
//    {
//        RAM_DATA.Il_buffer = Il_buf[i_buf_main];
//        RAM_DATA.Ul_buffer = Ul_buf[i_buf_main];
    
//    }
    
  }
}


//нажали кнопку Пуск-Старт
void START(void)
{
  if (RAM_DATA.FLAGS.BA.TR_ENABLE == 0)//если мы не в режиме пуска, еще не разрешали работу
  {
    //if (RAM_DATA.FLAGS.BA.DOUT1_FAIL == 0) //ошибок нет, флаг не выставлен
    //если есть ошибки - ничего не делаем
     if (RAM_DATA.FLAGS.BA.SYNCF | RAM_DATA.FLAGS.BA.SHC | RAM_DATA.FLAGS.BA.OPC |\
         RAM_DATA.FLAGS.BA.OVH | RAM_DATA.FLAGS.BA.THFOC | RAM_DATA.FLAGS.BA.THFAO |\
          RAM_DATA.FLAGS.BA.FQS | RAM_DATA.FLAGS.BA.MTZ_L | RAM_DATA.FLAGS.BA.MTZ_SH)
    {    }
    else //если нет ошибок
    {
      Init_soft(); //сброс всего в начальное значение
      RAM_DATA.FLAGS.BA.TR_ENABLE =1; //разрешили включение тиристоров - разрешать при кнопке Пуск   
    } 
  }
  else {} //на повторное нажатие пуска не реагируем и просто сбрасываем его
  
  RAM_DATA.FLAGS.BA.DI4_START = 0; //сбросили флаг нажатой кнопки, чтобы посторно не инициализировать все
}
 

void STOP(void)
{
  if (RAM_DATA.FLAGS.BA.WAIT == 0)//находимся не в режиме ожидания, первое нажатие стоп
  {
     RAM_DATA.FLAGS.BA.A_STOP = 1; // флаг повышения угла перед остановкой      ,
    // 
  }
  else //уже в состоянии ожидания, пришла деблокировка
  {
    Fail_Reset();
//флаг сброса остановки не сбрасываем, чтобы потом заново не проделывать весь путь остановки
  }
  RAM_DATA.FLAGS.BA.DI3_STOP = 0; //сбросили флаг нажатой кнопки
   
 
}


/*инициализация флагов и переменных предварительная*/
void Init_soft(void)
{
    /*дополнительная инициализация софта, которую потом отдельной цункцией запилить*/
  //все флаги вхо и вых обнулить
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
  
  RAM_DATA.FLAGS.BA.A_STOP = 0; // флаг повышения угла перед остановкой
  RAM_DATA.FLAGS.BA.WAIT = 0;//сброс флага состояния ожидания
  RAM_DATA.FLAGS.BA.SPARK = 0;//сброс флага искры
  RAM_DATA.FLAGS.BA.TR_ENABLE =0; //запретили включение тиристоров - разрешать при кнопке Пуск
  RAM_DATA.FLAGS.BA.TR1 = 0;//сброс флага тиристоров
  RAM_DATA.FLAGS.BA.TR2 = 0;//сброс флага тиристоров
  
  RAM_DATA.A = FLASH_DATA.Amax; //угол альфа максимальный
  RAM_DATA.Spark_cnt = 0; //сбросили счетчик, сколько длилась искра
  
  //зачем оно надо в рамке?
  RAM_DATA.Iz = FLASH_DATA.Iz; 
  RAM_DATA.Uz = FLASH_DATA.Uz;
  RAM_DATA.Az = FLASH_DATA.Amax;
  
  /*обнуление переменных, используемых в работе тиристоров в прерываниях*/
  NormalMode = true; //обнуление всех переменных, нужных в работе тиристоров
  Pause_Imp = 0;
 // SYNC_Yes = 0;               
                 
                 
}
//сброс всех аварий
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
  RAM_DATA.FLAGS.BA.DOUT3_SIGN = 0; //зажгли светик АВАРИЯ
}

//проверка на наличие аварий и действия при наличие какой-либо
void Fail_Check(void)
{
  if (  RAM_DATA.FLAGS.BA.MTZ_L | RAM_DATA.FLAGS.BA.MTZ_SH ) //авария мтз
  {
    RAM_DATA.FLAGS.BA.TR_ENABLE =0; //все запретили
    RAM_DATA.FLAGS.BA.WAIT = 1; //режим ожидания вкл
    RAM_DATA.FLAGS.BA.DOUT1_FAIL = 1; //выбили автомат
    RAM_DATA.FLAGS.BA.DOUT3_SIGN = 1; //зажгли светик АВАРИЯ
    RAM_DATA.A = FLASH_DATA.Amax; //угол альфа выставили максимальный
    return; //вышли и не проверяем остальное
  }
  if (RAM_DATA.FLAGS.BA.SYNCF)
  {
    RAM_DATA.FLAGS.BA.TR_ENABLE =0; //все запретили
    RAM_DATA.FLAGS.BA.WAIT = 1; //режим ожидания вкл
    RAM_DATA.FLAGS.BA.DOUT3_SIGN = 1; //зажгли светик АВАРИЯ
    RAM_DATA.A = FLASH_DATA.Amax; //угол альфа выставили максимальный
    return; //вышли и не проверяем остальное
  }
  //любая другая авария - не так страшно
  if ( RAM_DATA.FLAGS.BA.SHC | RAM_DATA.FLAGS.BA.OPC |\
  RAM_DATA.FLAGS.BA.OVH | RAM_DATA.FLAGS.BA.THFOC | RAM_DATA.FLAGS.BA.THFAO | RAM_DATA.FLAGS.BA.FQS)
  {
    RAM_DATA.FLAGS.BA.DOUT3_SIGN = 1; //зажгли светик АВАРИЯ
    STOP(); //вызвали состояние стоп/ожидание
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
