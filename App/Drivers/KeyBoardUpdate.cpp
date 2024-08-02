#include "DEFINES.h"
#include "ramdata.h"
#include "livecontrol.h"

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

u16 SPI_DIO_Inputs;

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

extern "C" void TIM2_IRQHandler(){
  TIM2->SR = 0;

  SPI_DIO_Processing();
  RAM_DATA.DI = SPI_DIO_Inputs;
  ctrlSysLive();
}