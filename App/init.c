#include "init.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "ramdata.h"
#include "flashdata.h"//глобальные константы и структура FLASH_DATA
#include "memutil.h"
#include "crc16.h"

#include "modbus/uart1rs485.h"


void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM1_Configuration(void);
void Systic_init(void);


ErrorStatus HSEStartUpStatus;

void Init (void)    
{  
  
  GPIO_Configuration();
  TIM1_Configuration(); //модбас
  
  usart1DMA_init();
  uart1rs485_init();

  NVIC_Configuration();
}


void GPIO_INIT_Configuration(){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //порт A                       USB_P
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
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
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA |\
                          RCC_AHBPeriph_GPIOB |\
                          RCC_AHBPeriph_GPIOC |\
                          RCC_APB2Periph_SYSCFG  ,  //RCC_APB2Periph_AFIO
                          ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* настраиваем ноги не привязанные к переферии, как open-drain*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  
    //порт B:                     LED_RUN    LED_LINK1      LED_ALARM    LED_LINK2 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

 //   GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap! 
  
  /* настраиваем ноги не привязанные к переферии, как push-pull*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;// GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //порт А:                        DIR1         DIR2       SPI1_LCLK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;// | GPIO_Pin_0 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  //порт A                       USB_P
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* настраиваем выходы переферии*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_1); //Tx 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); //Rx
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}
//******************************************************************************
//Таймер для работы с MODBUS два канала
void TIM1_Configuration(void){
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1  ,ENABLE);
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
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  
   /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);
  TIM1->SR = 0;

}



//******************************************************************************

void Systic_init(void)
{
  SysTick->LOAD  = 0xffff;      /* set reload register */  
  SysTick->VAL   = 0;           /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}
//******************************************************************************

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  //NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); //TODO
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
  
}
