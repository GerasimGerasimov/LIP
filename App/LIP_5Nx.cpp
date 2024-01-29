#include "LIP_5Nx.h"
#include "ramdata.h"
#include "stm32f10x_gpio.h"

LIP_5Nx::LIP_5Nx(){
    ++RAM_DATA.counter2;
}

void LIP_5Nx::setVal(){
    GPIOA->BRR = GPIO_Pin_6; //OFF
    GPIOA->BSRR = GPIO_Pin_6; //ON
}