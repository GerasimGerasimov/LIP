#include "init.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "ramdata.h"
#include "flashdata.h"//глобальные константы и структура FLASH_DATA
#include "memutil.h"
#include "crc16.h"
#include "stm32f0xx_syscfg.h"

#include "modbus/uart1rs485.h"

#define VECTOR_TABLE_SIZE (31 + 1 + 7 + 9)
extern volatile uint32_t __vector_table[VECTOR_TABLE_SIZE];

//размещение таблицы векторов прерываний в начало RAM
#pragma location = 0x20000000
volatile uint32_t ram_vector[VECTOR_TABLE_SIZE];

void GPIO_Configuration();
void NVIC_Configuration();
void TIM1_Configuration();
void Systic_init();
void remapMemory();
void TIM2_Configuration();
void TIM7_Configuration();
void SPI1_Configuration();
void SPI2_Configuration();
void DMA_Configuration();

uint8_t arr[15] = {0}; //TODO проверка DMA

void fillArr(){
  //uint16_t volatile* arrPtr = &RAM_DATA.Iz;
  for(int i = 0; i < 15; ++i){
    //arr[i] = ~((uint8_t)(RAM_DATA.Iz % 256)) ;
    //++arrPtr;
    arr[i] = ~((uint8_t)(1 << (i % 8)));
  }

}


ErrorStatus HSEStartUpStatus;

void Init(void){
  __disable_irq();
  remapMemory();

  GPIO_Configuration();
  TIM1_Configuration(); //модбас
  TIM2_Configuration();
  TIM7_Configuration();
  usart1DMA_init();
  uart1rs485_init();
  SPI1_Configuration();
  DMA_Configuration();
  SPI2_Configuration();

  NVIC_Configuration();
  __enable_irq();
}

void remapMemory(){
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE);
  //копирование вектора прерываний в начало RAM
  for(uint32_t i = 0; i < VECTOR_TABLE_SIZE; i++){//copy vector table
    ram_vector[i] = __vector_table[i];
  }
  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);//переназначение адресации прерываний на RAM
}

void SPI1_Configuration(){
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  /* SPI1 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  //SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}

void SPI2_Configuration(){
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE);
}

void GPIO_INIT_Configuration(){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //порт A                       USB_P
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap!  A13 и A14
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void GPIO_Configuration(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | \
    RCC_AHBPeriph_GPIOB | \
    RCC_AHBPeriph_GPIOC,
    ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* настраиваем ноги не привязанные к переферии, как open-drain*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    //порт B:                     LED_RUN    LED_LINK1      LED_ALARM    LED_LINK2 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

 //   GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap! 

  /* настраиваем ноги не привязанные к переферии, как push-pull*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;// GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //порт А:                        DIR1         DIR2       SPI1_LCLK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_0 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //                              SPI2_S/L    SPI2_CLK_INH
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //порт A                       USB_P
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* настраиваем выходы переферии*/
  //USART1
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //Tx 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); //Rx
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //SPI1
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //SPI2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//******************************************************************************
//Таймер для работы с MODBUS два канала
void TIM1_Configuration(void){

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 47;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xffff;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_Pulse = 0xFFFF;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  //TIM_OC2Init(TIM1, &TIM_OCInitStructure);

   /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  TIM1->SR = 0;

}

//100 Гц для DI кнопок на SPI2
void TIM2_Configuration(){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 100 - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 4800;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM2->SR = 0;
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//
  TIM_Cmd(TIM2, ENABLE);
}

void TIM7_Configuration(){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 1000 - 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 4800;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  TIM7->SR = 0;
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//
  TIM_Cmd(TIM7, ENABLE);
}

//******************************************************************************
//DMA на отправку в SPI1 на индикаторы
void DMA_Configuration(){
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  /* DMA1 channel1 configuration ----------------------------------------------*/
  //DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)arr;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  DMA_RemapConfig(DMA2, DMA2_CH4_SPI1_TX); //изменение конфигурации DMA для работы SPI1 с DMA2_Channel4
  /* Enable DMA2 Channel4 complete transfer interrupt */
  DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); //разрешение DMA работать с SPI
  //включение в другом месте
}

//void Systic_init(void)
//{
//  SysTick->LOAD  = 0xffff;      /* set reload register */  
//  SysTick->VAL   = 0;           /* Load the SysTick Counter Value */
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
//}
//******************************************************************************

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void){
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */
  //NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* Enable the TIM1 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 4;

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Ch4_7_DMA2_Ch3_5_IRQn; //DMA2_Channel4
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
