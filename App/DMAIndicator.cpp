#include "DMAIndicator.h"
#include "ramdata.h"
#include "init.h"

uint32_t addr = 0;

DMAIndicator& DMAIndicator::getInstance() {
    static DMAIndicator dma;
    return dma;
}

void DMAIndicator::setMemoryBaseAddr(uint32_t BaseAddr) {
    addr = BaseAddr;
}

DMAIndicator::DMAIndicator() {

}

//дма
extern "C" void DMA1_Channel3_IRQHandler(void)//прерывание вызывается, когда переданы все данные в SPI
{ 

  if (DMA_GetITStatus(DMA1_IT_TC3)){
    DMA_Cmd(DMA1_Channel3, DISABLE);

    //ожидание, пока SPI закончит отправку
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET){

    };
    if(addr != 0){
        DMA1_Channel3->CMAR = addr;
    }

    GPIO_SetBits(GPIOA, GPIO_Pin_6); //ON
    GPIO_ResetBits(GPIOA, GPIO_Pin_6); //OFF

    

    DMA_ClearITPendingBit(DMA1_IT_TC3);
    //DMA1->IFCR |= DMA_ISR_TCIF3; //сбрасываем флаг окончания обмена  
    DMA1_Channel3->CNDTR = 15;

    DMA_Cmd(DMA1_Channel3, ENABLE);
    
  }
}