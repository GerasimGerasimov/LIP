#include "STM32F10xUSART2.h"
#include "STM32F10x_Intmash_USART.h"
#include "ModbusMasterConf.h"
#include "bastypes.h"
#include "flashdata.h"
#include "ramdata.h"

static u16 TimeOut = 0;
static u8* ReplyPtr = 0;

const u32 U2BPS[]={  
  4800,// 0 
  9600,// 1 
 19200,// 2 
 57600,// 3 
115200,// 4
230400// 5
};

Intmash_Usart* STM32F10xUSART2::Usart = nullptr;
MBmasterSlotType* STM32F10xUSART2::Slot = nullptr;

STM32F10xUSART2::STM32F10xUSART2(Intmash_Usart* usart, MBmasterSlotType* slot) {
    Usart = usart;
    Slot = slot;
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
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 7200;
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

void STM32F10xUSART2::setCondition(u16 timeOut, u8 *replyPtr){
    TimeOut = timeOut;
    ReplyPtr = replyPtr;
}

Intmash_Usart *STM32F10xUSART2::getUsart(){
    return Usart;
}

MBmasterSlotType *STM32F10xUSART2::getSlot(){
    return Slot;
}

void StopMasterTimer(){
    TIM_Cmd(TIM3, DISABLE);
}

void SetMasterTimer(u16 delay){
    TIM3->CNT = 0;
    TIM3->ARR = delay;//зарядить на нужную паузу.
    TIM7->SR = 0; //снять флаг прерывания
    TIM_Cmd(TIM3, ENABLE);//запустили снова, ждем когда пауза сработает.
}

extern "C" void TIM3_IRQHandler(){
  TIM3->SR = 0;
  ++RAM_DATA.counter[2];
  
  MBmasterSlotType* slot = STM32F10xUSART2::getSlot();
  if(slot->OnTimeOut){
    slot->OnTimeOut();
  }
    StopMasterTimer();
}

extern "C" void USART2_IRQHandler(void){
    Intmash_Usart* usart = STM32F10xUSART2::getUsart();
    u16 TransferStatus =  UsartTxRxFinish(usart);
    if (TransferStatus == 0){
        SetMasterTimer(TimeOut);//установили таймер на ожидание таймаута
        UsartRecieve(usart, ReplyPtr);
    }
    else{
        StopMasterTimer();
        MBmasterSlotType* slot = STM32F10xUSART2::getSlot();
        if(slot->OnRecieve){
            slot->InBufLen = TransferStatus;
            slot->OnRecieve();
        }
    }
  
}