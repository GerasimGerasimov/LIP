#include "STM32F10xUSART2.h"
#include "STM32F10x_Intmash_USART.h"
#include "com_master_driver.h"
#include "bastypes.h"
#include "flashdata.h"


const u32 U2BPS[]={  
  4800,// 0 
  9600,// 1 
 19200,// 2 
 57600,// 3 
115200,// 4
230400// 5
};



STM32F10xUSART2::STM32F10xUSART2(Intmash_Usart* usart) {
    
    Usart = usart;
    
    initUsart();
    initTIM();
    startNVIC();
}

void STM32F10xUSART2::initUsart() {
    bavu16 InterfaceSettings;
    InterfaceSettings = FLASH_DATA.MODBUS2;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    Usart->USARTx = USART2;
    Usart->USART_BaudRate = U2BPS[FLASH_DATA.MODBUS2.b[1]];
    Usart->USART_StopBits = USART_StopBits_1;
    Usart->USART_Parity = USART_Parity_No;
    Usart->DMAy_StreamRX = DMA1_Channel6;
    Usart->DMAy_StreamTX = DMA1_Channel7;
    Usart->DMA_FLAGS_RX = DMA_IFCR_CTCIF6 | DMA_IFCR_CGIF6 | DMA_IFCR_CHTIF6 | DMA_IFCR_CTEIF6;
    Usart->DMA_FLAGS_TX = DMA_IFCR_CTCIF7 | DMA_IFCR_CGIF7 | DMA_IFCR_CHTIF7 | DMA_IFCR_CTEIF7;
    Usart->PolarityDIR = POLARITY_HI;
    Usart->GPIOxDIR = GPIOA;
    Usart->GPIO_PinDIR = GPIO_Pin_0;
    UsartDriverInit(Usart);
}

void STM32F10xUSART2::initTIM() {
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xffff;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM3->SR = 0;
}

void STM32F10xUSART2::startNVIC() {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void STM32F10xUSART2::StopTimer(){
    TIM_Cmd(TIM3, DISABLE);
}

void STM32F10xUSART2::SetTimer(u16 delay){
    TIM3->CNT = 0;
    TIM3->ARR = delay;//зарядить на нужную паузу.
    TIM3->SR = 0; //снять флаг прерывания
    TIM_Cmd(TIM3, ENABLE);//запустили снова, ждем когда пауза сработает.
}

extern "C" void TIM3_IRQHandler(){
  TIM3->SR = 0;
  ComMasterDriver::TIMHandler();
}

extern "C" void USART2_IRQHandler(void){
    ComMasterDriver::interruptHandler();
  
}