#include "DMAIndicator.h"
#include "ramdata.h"
#include "Page/Page.h"
#include "Indicator/Indicator.h"
#include "Buffer/Buffer.h"


DMAIndicator& DMAIndicator::getInstance() {
    static DMAIndicator dma;
    return dma;
}

void DMAIndicator::setMemoryBaseAddr(Buffer& buf) {
  
    DMA1_Channel3->CMAR = buf.getAddrBuffer();
}

DMAIndicator::DMAIndicator() {

}

void DMAIndicator::bringOutValue(){
  GPIO_SetBits(GPIOA, GPIO_Pin_6); //ON
  GPIO_ResetBits(GPIOA, GPIO_Pin_6); //OFF
}

void DMAIndicator::DMAstart(uint32_t BufferSize){
  DMA1_Channel3->CNDTR = BufferSize;
  DMA_Cmd(DMA1_Channel3, ENABLE);
}

//дма
extern "C" void DMA1_Channel3_IRQHandler(void)//прерывание вызывается, когда переданы все данные в SPI
{ 

  if (DMA_GetITStatus(DMA1_IT_TC3)){
    DMA_Cmd(DMA1_Channel3, DISABLE);

        updateDMA();

    //ожидание, пока SPI закончит отправку
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){

    };

    DMAIndicator::bringOutValue();

    DMA_ClearITPendingBit(DMA1_IT_TC3);
    //DMA1->IFCR |= DMA_ISR_TCIF3; //сбрасываем флаг окончания обмена  
    DMA1_Channel3->CNDTR = 15;

    DMA_Cmd(DMA1_Channel3, ENABLE);
    
  }
}