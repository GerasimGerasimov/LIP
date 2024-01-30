#include "Indicator.h"
#include "stm32f10x_gpio.h"

void Indicator::bringOutValue(){
    GPIOA->BSRR = GPIO_Pin_6; //ON
    GPIOA->BRR = GPIO_Pin_6; //OFF
}