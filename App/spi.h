#ifndef SPI_H
#define SPI_H

#include "stm32f10x.h"//библиотека STM

extern void spi_send(uint16_t data);
extern uint16_t spi_receve(void);


    
#endif