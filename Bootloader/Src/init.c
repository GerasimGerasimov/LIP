#include "init.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "ramdata.h"
#include "flashdata.h"//глобальные константы и структура FLASH_DATA
#include "memutil.h"
#include "crc16.h"

#include "modbus/uart1rs485.h"
#include "modbus/uart2rs485.h"//связь по 485 интерфейсу, по протоколу MODBUS (клиент)
#include "fram/fram.h"
#include "DEFINES.h" //все основные, относящиеся только к плате дефайны


void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM1_Configuration(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);
void TIM4_Configuration(void);
void ADC_Configuration (void);
void Systic_init(void);
void EXTI_init(void);
void check_flash (void);





ErrorStatus HSEStartUpStatus;

void Init (void)    
{  
  
  GPIO_Configuration();
  TIM1_Configuration(); //модбас


  EXTI_init(); 
  
  usart1DMA_init();
  uart1rs485_init();
  usart2DMA_init();
  uart2rs485_init();
 
  TIM_Cmd(TIM3, ENABLE);

  NVIC_Configuration();

  
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
  GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14 | GPIO_Pin_13;// | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap!  A13 и A14
    //порт B:                     LED_RUN    LED_LINK1      LED_ALARM    LED_LINK2      FR_FCS       FR_DATA        FR_CLK      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7 | GPIO_Pin_8;// | GPIO_Pin_13 | GPIO_Pin_14;// | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap! 
  
  /* настраиваем ноги не привязанные к переферии, как push-pull*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  //порт А:                        DIR1         DIR2       SPI1_LCLK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_0 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* настраиваем входы переферии как input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  //порт А:                     UART1_Rx       UART2_Rx     DI5_TERM       
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;// | GPIO_Pin_15; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  //порт В:  дискретные входы     DI3_STOP     DI4_START       SYNC          TR1           TR2             
  GPIO_InitStructure.GPIO_Pin =  /* GPIO_Pin_4 | */ GPIO_Pin_3;// | GPIO_Pin_11;// | GPIO_Pin_1 | GPIO_Pin_0;  
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
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
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //порт B:                     SPI2_SCK     SPI2_MISO
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14;  
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);//remap! T2_CH3->PB.10  




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


//*****************************************************************************
//T2 канал TIM2_CH3 - имп. управления тиристорами - шим ноги, один канал
void TIM2_Configuration(void){
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//управление ШИМ высокочастотного наполнения импульсов управления
//тиристорами находится на таймере TIM2
//T = TIM2_CH3 PB.10 remap!!
//нужно сформировать (когда резрешено) наполнение 200кГц со скважностью 30%
//TIM2 сидит на APB1 частота которой 72 МГц
//какое число нужно зарядить в таймеры чтоб получить 200кГц
//72 МГц/200000 = 360 - это будет период таймера TIM2 (TIM_Period)
//теперь, какое число нужно зарядить в каналы 1 и 2 чтобы получить сважность 30%
//360 * 0,3 = 108 (это будет TIM_Pulse)
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period =360;// TIM2_PERIOD;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* T PWM2 Mode configuration: Channel 3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//TIM2_CH_3_4_Q;//TIM2_CH_3_4_Q;//OFF
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  /* TIMER 2 IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

  /* TIMER 2enable counter */
  TIM_Cmd(TIM2, ENABLE);
//  TIM2->CR1 &= ~TIM_CR1_CEN;//остановить таймер
//   TIM2->CNT = 0xffff; //уровень должен быть 1
  
 
}
//******************************************************************************
//Таймер тактирования АЦП (частота переполнения и вых триггера 10 кГц)
void TIM3_Configuration(void){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
   /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 7200;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High; 
  TIM_OCInitStructure.TIM_Pulse = 5000;
 // TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
  
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_OC4Ref); //1

  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
   /* TIM3 counter enable */
  //TIM_Cmd(TIM3, ENABLE);
  TIM3->SR = 0;  
}
//******************************************************************************
//таймер для управлениями тиристорами - сифу, угол (1тик = 1мкс) два канала
void TIM4_Configuration(void){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xffff;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  
   /* TIM4 counter enable */
  TIM4->CCR1 = 0; //длинна угла
  TIM4->CCR2 =  0; //длинна угла +длинна импульса управления

  TIM4->SR = 0;
    TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);//
  TIM_Cmd(TIM4, DISABLE);


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

    /* Enable the TIM2 gloabal Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   
       /* Enable the TIM4 gloabal Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
  
  /* Enable DMA1 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the EXTI11 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI_init(void){
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
  
  EXTI_InitTypeDef  EXTI_InitStructure;
  
  /* Configure EXTI_Line11 to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling; //по спаду и фронту
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
}


void check_flash (void)
{
  //функция проверяет целостность уставок во ФЛЕШ
  u8 *crcl1;
  u8 *crcl2;
  u8 *crch1;
  u8 *crch2;
  
  RAM_DATA.FLAGS.BA.backup_error = 0;
  RAM_DATA.FLAGS.BA.flash_error = 0;
  if (crc16((u8*)&FLASH_DATA, FlashTmpBufferSize))
  {    
    //контрольная сумма не сошлась, сектор битый или после прошивки
    if (crc16((u8*)&BKFLASH_DATA, FlashTmpBufferSize)) 
    {
      RAM_DATA.FLAGS.BA.flash_error = 1; //оба сектора битые или после прошивки
      RAM_DATA.FLAGS.BA.backup_error = 1; 
    }
    else  
    {
      FlashSectorWrite((u32)&FLASH_DATA, (u32)&BKFLASH_DATA);//пытаемся восстановить из бэкапа
      if (crc16((u8*)&FLASH_DATA, FlashTmpBufferSize)) RAM_DATA.FLAGS.BA.flash_error = 1; //не получилось, ставим ошибку
    }    
  }
  else
  {
     //нормальная ситуация, сектор в порядке
    if (crc16((u8*)&BKFLASH_DATA, FlashTmpBufferSize)) 
    {
      FlashSectorWrite((u32)&BKFLASH_DATA, (u32)&FLASH_DATA);//резервный сектор битый, пытаемся восстановить
      if (crc16((u8*)&BKFLASH_DATA, FlashTmpBufferSize)) RAM_DATA.FLAGS.BA.backup_error = 1; //не получилось       
    }
    else {//оба сектора в порядке
      crcl1 = (u8*)&FLASH_DATA + (FlashTmpBufferSize-2);
      crcl2 = (u8*)&BKFLASH_DATA + (FlashTmpBufferSize-2);
      crch1 = (u8*)&FLASH_DATA + (FlashTmpBufferSize-1);
      crch2 = (u8*)&BKFLASH_DATA + (FlashTmpBufferSize-1);
      if ((*crcl1 != *crcl2)||(*crch1 != *crch2)){//если контрольные суммы у секторов разные
        FlashSectorWrite((u32)&BKFLASH_DATA, (u32)&FLASH_DATA);//запишем в бекап основной сектор
      }
    }
  }
}






