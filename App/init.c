#include "init.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "ramdata.h"
#include "flashdata.h"//���������� ��������� � ��������� FLASH_DATA
#include "memutil.h"
#include "crc16.h"

#include "modbus/uart1rs485.h"
#include "modbus/uart2rs485.h"//����� �� 485 ����������, �� ��������� MODBUS (������)

#include "DEFINES.h" //��� ��������, ����������� ������ � ����� �������


void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM1_Configuration(void);
//void TIM2_Configuration(void);
//void TIM3_Configuration(void);
void TIM4_Configuration(void);
void Systic_init(void);
void EXTI_init(void);
void DMA_Configuration (void);
void SPI1_Configuration(void);
void check_flash (void);
void SPI2_Configuration();


uint8_t arr[15]; //TODO �������� DMA

void fillArr(){
  //uint16_t volatile* arrPtr = &RAM_DATA.Iz;
  for(int i = 0; i < 15; ++i){
    //arr[i] = ~((uint8_t)(RAM_DATA.Iz % 256)) ;
    //++arrPtr;
    arr[i] = ~((uint8_t)(1 << (i % 8)));
  }

}

ErrorStatus HSEStartUpStatus;

void Init (void)    
{  
  fillArr();
  
  GPIO_Configuration();
  TIM1_Configuration(); //������
  //TIM2_Configuration();//��� ���������
  //TIM3_Configuration(); //������������ ���
  TIM4_Configuration();// ������ ����������, ������������ ��� ��������� ������� ����/���
  
  //ADC_Configuration();

  EXTI_init(); 
  
  usart1DMA_init();
  uart1rs485_init();
  
  uart2rs485_init();
 
  TIM_Cmd(TIM3, ENABLE);
  SPI1_Configuration();
  SPI2_Configuration();
  DMA_Configuration();
  NVIC_Configuration();
  __enable_irq();
  /*
  check_fram();//��������� ���� ����������
  check_flash();//��������� ����-������� ������
  //�� ����������� �������� ������ ����������� ����������������� ��������
  if (RAM_DATA.FLAGS.BA.flash_error) mem_status = 2;
  else mem_status = 0;
  if (RAM_DATA.FLAGS.BA.fram_error) mem_status++;
  
  switch (mem_status) {
    case 0x00: crcl = (u8*)&FLASH_DATA + (FlashTmpBufferSize-2);      
               crch = (u8*)&FLASH_DATA + (FlashTmpBufferSize-1);
               if ((*crcl != Fram_Buffer[FramTmpBufferSize-2])||(*crch != Fram_Buffer[FramTmpBufferSize-1])){//���� ����������� ����� � �������� ������
                FlashSectorWrite((u32)&FLASH_DATA, (u32)&Fram_Buffer);//������� � �������� ������
                FlashSectorWrite((u32)&BKFLASH_DATA, (u32)&Fram_Buffer);
                check_flash();
               }
               break;
    case 0x01: fram_wr_massive((u8*)&FLASH_DATA, FramTmpBufferSize, 0x00, fram_sector);
               fram_wr_massive((u8*)&FLASH_DATA, FramTmpBufferSize, 0x00, fram_bkp);
               check_fram();
               break;
    case 0x02: FlashSectorWrite((u32)&FLASH_DATA, (u32)&Fram_Buffer);
               FlashSectorWrite((u32)&BKFLASH_DATA, (u32)&Fram_Buffer);
               check_flash();
               break;
    default:   break;  
    
  }
  */
  
}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Configuration(void){
  SPI_InitTypeDef  SPI_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE);
  /* SPI1 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  //SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1 NSS output for master mode */
  //SPI_SSOutputCmd(SPI1, ENABLE);
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

void GPIO_Configuration(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |\
                          RCC_APB2Periph_GPIOB |\
                          RCC_APB2Periph_GPIOC |\
                          RCC_APB2Periph_AFIO  ,
                          ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����������� ���� �� ����������� � ���������, ��� open-drain*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  //���� �:                          Dout1           Dout2        Dout3 
  GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14 | GPIO_Pin_13;// | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap!  A13 � A14
    //���� B:                     LED_RUN    LED_LINK1      LED_ALARM    LED_LINK2      FR_FCS       FR_DATA        FR_CLK      
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6  | GPIO_Pin_7 | GPIO_Pin_8;// | GPIO_Pin_13 | GPIO_Pin_14;// | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//remap! 
  
  /* ����������� ���� �� ����������� � ���������, ��� push-pull*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  //���� �:                        DIR1         DIR2       SPI1_LCLK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_0 | GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* ����������� ����� ��������� ��� input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  //���� �:                     UART1_Rx       UART2_Rx     DI5_TERM       
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;// | GPIO_Pin_15; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  //���� �:  ���������� �����     DI3_STOP     DI4_START       SYNC          TR1           TR2             
  GPIO_InitStructure.GPIO_Pin =  /* GPIO_Pin_4 | */ GPIO_Pin_3;// | GPIO_Pin_11;// | GPIO_Pin_1 | GPIO_Pin_0;  
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
    //���� C:  ���������� �����       DI1           DI2_BURNING         
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_15;  
//  GPIO_Init(GPIOC, &GPIO_InitStructure);  
 
    /* Connect EXTI_Line11 to SYNC input PB.11 GPIO Pin */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
  
  /* ����������� ���� ���������� ��������*/
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
  //���� �:                     ADC_I         ADC_U      ADC_Ish   
//  GPIO_InitStructure.GPIO_Pin = 
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ����������� ������ ��������� ��� push-pull */
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //���� �:                       UART1_Tx     UART2_Tx    SPI1_SCK     SPI1_MOSI  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //���� B:                     SPI2_SCK     SPI2_MISO
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14;  
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);//remap! T2_CH3->PB.10  




}
//******************************************************************************
//������ ��� ������ � MODBUS ��� ������
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
//T2 ����� TIM2_CH3 - ���. ���������� ����������� - ��� ����, ���� �����
//void TIM2_Configuration(void){
//  
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef  TIM_OCInitStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
////���������� ��� ���������������� ���������� ��������� ����������
////����������� ��������� �� ������� TIM2
////T = TIM2_CH3 PB.10 remap!!
////����� ������������ (����� ���������) ���������� 200��� �� ����������� 30%
////TIM2 ����� �� APB1 ������� ������� 72 ���
////����� ����� ����� �������� � ������� ���� �������� 200���
////72 ���/200000 = 360 - ��� ����� ������ ������� TIM2 (TIM_Period)
////������, ����� ����� ����� �������� � ������ 1 � 2 ����� �������� ��������� 30%
////360 * 0,3 = 108 (��� ����� TIM_Pulse)
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  /* Time base configuration */
//  TIM_TimeBaseStructure.TIM_Period =360;// TIM2_PERIOD;
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//
//  /* T PWM2 Mode configuration: Channel 3 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 0;//TIM2_CH_3_4_Q;//TIM2_CH_3_4_Q;//OFF
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
//  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
//
//  /* TIMER 2 IT enable */
//    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
//
//  /* TIMER 2enable counter */
//  TIM_Cmd(TIM2, ENABLE);
////  TIM2->CR1 &= ~TIM_CR1_CEN;//���������� ������
////   TIM2->CNT = 0xffff; //������� ������ ���� 1
//  
// 
//}
//******************************************************************************
//������ ������������ ��� (������� ������������ � ��� �������� 10 ���)
//void TIM3_Configuration(void){
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef        TIM_OCInitStructure;
//  
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//   /* Time Base configuration */
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseStructure.TIM_Period = 7200;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//  
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High; 
//  TIM_OCInitStructure.TIM_Pulse = 5000;
// // TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//  
//    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
//  
//  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_OC4Ref); //1
//
//  TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
//   /* TIM3 counter enable */
//  //TIM_Cmd(TIM3, ENABLE);
//  TIM3->SR = 0;  
//}
//******************************************************************************
//������ ��� ������������ ����������� - ����, ���� (1��� = 1���) ��� ������
void TIM4_Configuration(void){
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef        TIM_OCInitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
   /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 9;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 7200;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
//  TIM_OCInitStructure.TIM_Pulse = 0;
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
//  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//  
//   /* TIM4 counter enable */
//  TIM4->CCR1 = 0; //������ ����
//  TIM4->CCR2 =  0; //������ ���� +������ �������� ����������

  TIM4->SR = 0;
    TIM_ITConfig(TIM4, TIM_IT_Update /* | TIM_IT_CC2 */, ENABLE);//
  TIM_Cmd(TIM4, ENABLE);
   
  

}
////******************************************************************************
//
//void Systic_init(void)
//{
//  SysTick->LOAD  = 0xffff;      /* set reload register */  
//  SysTick->VAL   = 0;           /* Load the SysTick Counter Value */
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
//}
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
//��� ��� SPI1 (�������� ������ �� ����������)
void DMA_Configuration (void){
  DMA_InitTypeDef DMA_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  /* DMA1 channel1 configuration ----------------------------------------------*/
  //DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(SPI1->DR);
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) arr;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  
  /* Enable DMA1 Channel1 complete transfer interrupt */
  DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
  //DMA_Cmd(DMA1_Channel3, ENABLE);

  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); // ���������� DMA �������� � SPI
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#define APP_START_ADDR 0x08008000

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

//#ifdef  VECT_TAB_RAM  
//  /* Set the Vector Table base location at 0x20000000 */ 
//  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
//#else  /* VECT_TAB_FLASH  */
//  /* Set the Vector Table base location at 0x08000000 */ 
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
//#endif
  
  SCB->VTOR = APP_START_ADDR;//��������� ������ ������� ���������� �� ���������� ������
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //������ ��� - ������ 2 ���� ��� ���������� � 2 ��� �������������
  /* Enable the TIM1 gloabal Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

    /* Enable the TIM2 gloabal Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);
//   
//       /* Enable the TIM4 gloabal Interrupt */
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
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Falling; //�� ����� � ������
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
}


void check_flash (void)
{
  //������� ��������� ����������� ������� �� ����
  u8 *crcl1;
  u8 *crcl2;
  u8 *crch1;
  u8 *crch2;
  
  RAM_DATA.FLAGS.BA.backup_error = 0;
  RAM_DATA.FLAGS.BA.flash_error = 0;
  if (crc16((u8*)&FLASH_DATA, FlashTmpBufferSize))
  {    
    //����������� ����� �� �������, ������ ����� ��� ����� ��������
    if (crc16((u8*)&BKFLASH_DATA, FlashTmpBufferSize)) 
    {
      RAM_DATA.FLAGS.BA.flash_error = 1; //��� ������� ����� ��� ����� ��������
      RAM_DATA.FLAGS.BA.backup_error = 1; 
    }
    else  
    {
      FlashSectorWrite((u32)&FLASH_DATA, (u32)&BKFLASH_DATA);//�������� ������������ �� ������
      if (crc16((u8*)&FLASH_DATA, FlashTmpBufferSize)) RAM_DATA.FLAGS.BA.flash_error = 1; //�� ����������, ������ ������
    }    
  }
  else
  {
     //���������� ��������, ������ � �������
    if (crc16((u8*)&BKFLASH_DATA, FlashTmpBufferSize)) 
    {
      FlashSectorWrite((u32)&BKFLASH_DATA, (u32)&FLASH_DATA);//��������� ������ �����, �������� ������������
      if (crc16((u8*)&BKFLASH_DATA, FlashTmpBufferSize)) RAM_DATA.FLAGS.BA.backup_error = 1; //�� ����������       
    }
    else {//��� ������� � �������
      crcl1 = (u8*)&FLASH_DATA + (FlashTmpBufferSize-2);
      crcl2 = (u8*)&BKFLASH_DATA + (FlashTmpBufferSize-2);
      crch1 = (u8*)&FLASH_DATA + (FlashTmpBufferSize-1);
      crch2 = (u8*)&BKFLASH_DATA + (FlashTmpBufferSize-1);
      if ((*crcl1 != *crcl2)||(*crch1 != *crch2)){//���� ����������� ����� � �������� ������
        FlashSectorWrite((u32)&BKFLASH_DATA, (u32)&FLASH_DATA);//������� � ����� �������� ������
      }
    }
  }
}






