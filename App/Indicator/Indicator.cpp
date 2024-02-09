#include "Indicator.h"
#include "stm32f10x_gpio.h"

void Indicator::bringOutValue(){
    GPIO_SetBits(GPIOA, GPIO_Pin_6); //ON
    GPIO_ResetBits(GPIOA, GPIO_Pin_6); //OFF
}

uint8_t Indicator::getDataSize() {
    return DataSize;
}
