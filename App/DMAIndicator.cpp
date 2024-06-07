#include "DMAIndicator.h"
#include "ramdata.h"
#include "Page/Page.h"
#include "Indicator/Indicator.h"
#include "Buffer/Buffer.h"
#include "Router/Router.h"


DMAIndicator& DMAIndicator::getInstance(){
    static DMAIndicator dma;
    return dma;
}

void DMAIndicator::setMemoryBaseAddr(Buffer& buf){

    DMA2_Channel4->CMAR = buf.getAddrBuffer();
}

DMAIndicator::DMAIndicator(){}

void DMAIndicator::bringOutValue(){
    GPIO_SetBits(GPIOA, GPIO_Pin_6); //ON
    GPIO_ResetBits(GPIOA, GPIO_Pin_6); //OFF
}

void DMAIndicator::DMAstart(uint32_t BufferSize){
    DMA2_Channel4->CNDTR = BufferSize;
    DMA_Cmd(DMA2_Channel4, ENABLE);
}

void DMAIndicator::DMAstop(){
    DMA_Cmd(DMA2_Channel4, DISABLE);
}

//дма
extern "C" void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler()//прерывание вызывается, когда переданы все данные в SPI
{

    if(DMA_GetITStatus(DMA2_IT_TC4)){
        DMA_Cmd(DMA2_Channel4, DISABLE);

        //ожидание, пока SPI закончит отправку
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){
            ++RAM_DATA.counter[0];
        };

        DMAIndicator::bringOutValue();
        DMA_ClearITPendingBit(DMA2_FLAG_TC4);//сбрасываем флаг окончания обмена  
        Router::getInstance().setEmptyBufferStatus();
    }
}

extern "C" void TIM7_IRQHandler(){
    TIM7->SR = 0;
    if (Router::getInstance().isFillBuffer()){
        DMAIndicator::getInstance().DMAstart(Router::getInstance().getBufferSize());
    }
}