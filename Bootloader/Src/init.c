#include "init.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "ramdata.h"
#include "flashdata.h"//глобальные константы и структура FLASH_DATA
#include "memutil.h"
#include "crc16.h"

#include "modbus/uart1rs485.h"
#include "DEFINES.h" //все основные, относящиеся только к плате дефайны


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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //порт A                       USB_P
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap!  A13 и A14
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
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |\
                          RCC_APB2Periph_GPIOB |\
                          RCC_APB2Periph_GPIOC |\
                          RCC_APB2Periph_AFIO  ,
                          ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* настраиваем ноги не привязанные к переферии, как open-drain*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  //порт А:                          Dout1           Dout2        Dout3 
 // GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_13 | GPIO_Pin_13;// | GPIO_Pin_12;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    //порт B:                     LED_RUN    LED_LINK1      LED_ALARM    LED_LINK2      FR_FCS       FR_DATA        FR_CLK      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7 | GPIO_Pin_8;// | GPIO_Pin_13 | GPIO_Pin_14;// | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  
 //   GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap! 
  
  /* настраиваем ноги не привязанные к переферии, как push-pull*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  //порт А:                        DIR1         DIR2       SPI1_LCLK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;// | GPIO_Pin_0 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* настраиваем входы переферии как input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  //порт А:                     UART1_Rx       UART2_Rx     DI5_TERM       
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;// | GPIO_Pin_3;// | GPIO_Pin_15; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  //порт В:  дискретные входы     DI3_STOP     DI4_START       SYNC          TR1           TR2             
  //GPIO_InitStructure.GPIO_Pin =  /* GPIO_Pin_4 | */ GPIO_Pin_3;// | GPIO_Pin_11;// | GPIO_Pin_1 | GPIO_Pin_0;  
  //GPIO_Init(GPIOB, &GPIO_InitStructure); 
    //порт C:  дискретные входы       DI1           DI2_BURNING         
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;  
//  GPIO_Init(GPIOC, &GPIO_InitStructure);  
 
    /* Connect EXTI_Line11 to SYNC input PB.11 GPIO Pin */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
  
  /* настраиваем ноги аналоговых сигналов*/
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
  //порт А:                     ADC_I         ADC_U      ADC_Ish   
//  GPIO_InitStructure.GPIO_Pin = 
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* настраиваем выходы переферии как push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //порт А:                       UART1_Tx     UART2_Tx    SPI1_SCK     SPI1_MOSI  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;// | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //порт B:                     SPI2_SCK     SPI2_MISO
 // GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14;  
  //GPIO_Init(GPIOB, &GPIO_InitStructure); 
  //GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);//remap! T2_CH3->PB.10  




}
//******************************************************************************
//Таймер для работы с MODBUS два канала
void TIM1_Configuration(void){
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1  ,ENABLE);
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 71;
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

//*******************************************************************************
void RTC_init(void)
{
        RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
	PWR->CR |= PWR_CR_DBP;

	if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
	{
		RCC->BDCR |= RCC_BDCR_BDRST;
		RCC->BDCR &= ~RCC_BDCR_BDRST;
		RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;

		RTC->CRL |= RTC_CRL_CNF;
		RTC->PRLL = 0x7FFF;
		RTC->CNTH = 0;
		RTC->CNTL = 16;
		RTC->CRL &= ~RTC_CRL_CNF;               

		RCC->BDCR |= RCC_BDCR_LSEON;
		while ((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON)
		{

		}

		RTC->CRL &= (uint16_t)~RTC_CRL_RSF;
		while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF)
		{

		}
                
	}
        //PWR->CR &= ~PWR_CR_DBP;
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
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //группа два - значит 2 бита для приоритета и 2 для подприоритета
  /* Enable the TIM1 gloabal Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}
